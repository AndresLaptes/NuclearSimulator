#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Proton.h"
#include "Neutron.h"
#include "Electron.h"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

Window::Window(uint width, uint height) {
    if (!glfwInit()) cout << "Error" << endl; //Error

    fullscrean = false;
    velocidad = 1.0f;
    rightMousePressed = false;
    lastMouseX = lastMouseY = 0.0;
    yaw = pitch = 0.0;
    radius = 5.0;
    window = glfwCreateWindow(width, height, "Nuclear Simulator", nullptr, nullptr);

    if (!window) cout << "Error crear window" << endl;

    glfwGetWindowPos(window, &windowPosX, &windowPosY);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);
    inicializarMapa();
}

void Window::mouseButtonCallback(GLFWwindow* win, int button, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        self->rightMousePressed = (action == GLFW_PRESS);
        if (self->rightMousePressed) {
            glfwGetCursorPos(win, &self->lastMouseX, &self->lastMouseY);
        }
    }
}


void Window::mouseCallback(GLFWwindow* win, double xpos, double ypos) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (!self->rightMousePressed) return;

    float sensitivity = 0.005f;
    float dx = xpos - self->lastMouseX;
    float dy = ypos - self->lastMouseY;

    self->yaw   += dx * sensitivity;
    self->pitch += dy * sensitivity;

    self->pitch = glm::clamp(self->pitch, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

    self->lastMouseX = xpos;
    self->lastMouseY = ypos;

    self->updateCamera(); 
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->updateViewport(width, height);
    }
}

void Window::scrollCallback(GLFWwindow* win, double xoffset, double yoffset) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (!self) return;

    glm::vec3 direction = glm::normalize(self->cameraTarget - self->cameraPos);

    float zoomAmount = yoffset * 1.5f; // Ajusta la velocidad si quieres variable en un futuro
    self->cameraPos += direction * zoomAmount;
}

void Window::updateViewport(int width, int height) {
    currentWidth = width;
    currentHeight = height;
    
    glViewport(0, 0, width, height);
    
    // Recalcular matriz de proyección con el nuevo aspect ratio
    float aspectRatio = (float)width / (float)height;
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}

void Window::endWindow() {
    glfwTerminate();
}

void Window::move(glm::vec3 dir) {
    float velocityDelta = velocidad * deltaTime;
    cameraPos += dir * velocityDelta;
    cameraTarget += dir * velocityDelta;
}

void Window::processHeldKeys() {
    for (const auto& pair : keysPressed) {
        if (pair.second) { // Tecla presionada
            auto it = keyActions.find(pair.first);
            if (it != keyActions.end()) {
                it->second();
            }
        }
    }
}


void Window::Fullscrean() {
    fullscrean = !fullscrean;

    if (fullscrean) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwGetWindowPos(window, &windowPosX, &windowPosY);
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        glfwSetWindowMonitor(window, nullptr, windowPosX, windowPosY, windowWidth, windowHeight, 0);
    }

}

void Window::KeyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    if (!self) return;

    if (action == GLFW_PRESS) {
        self->keysPressed[key] = true;

        auto it = self->oneTimeKeyActions.find(key);
        if (it != self->oneTimeKeyActions.end()) {
            it->second();
        }
    } else if (action == GLFW_RELEASE) {
        self->keysPressed[key] = false;
    }
}

void Window::velocidadCamera(float p) {
    velocidad += p;
}

void Window::inicializarMapa() {
    oneTimeKeyActions[GLFW_KEY_F11] = [this]() {
        Fullscrean();
    };

    keyActions[GLFW_KEY_A] = [this]() {
        move(glm::vec3(-1, 0, 0));
    };

    keyActions[GLFW_KEY_D] = [this]() {
        move(glm::vec3(1, 0, 0));
    };

    keyActions[GLFW_KEY_W] = [this]() {
        move(glm::vec3(0, 0, -1));
    };

    keyActions[GLFW_KEY_S] = [this]() {
        move(glm::vec3(0, 0, 1));
    };

    keyActions[GLFW_KEY_SPACE] = [this]() {
        move(glm::vec3(0, 1, 0));
    };

    keyActions[GLFW_KEY_LEFT_SHIFT] = [this]() {
        move(glm::vec3(0, -1, 0));
    };

    keyActions[GLFW_KEY_Q] = [this]() { //Temporales
        velocidadCamera(5.0);
    };

    keyActions[GLFW_KEY_E] = [this]() { //Temporales
        velocidadCamera(-5.0);
    };

    oneTimeKeyActions[GLFW_KEY_ESCAPE] = [this]() {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    };
}

void Window::drawObjects() {
    for (auto p : particulas) {
        glm::mat4 mv = view * p->getModelMatrix();
        glm::mat4 mvp = projection * mv;
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(mv)));


        shaders->setMat4("modelViewProjectionMatrix", mvp);
        shaders->setMat4("modelViewMatrix", mv);
        shaders->setMat3("normalMatrix", normalMatrix);

        glm::vec3 color;
        switch (p->getTipo()) {
            case 1: color = glm::vec3(1, 0, 0); break; // Proton -> rojo
            case 2: color = glm::vec3(0.8, 0.8, 0.8); break; // Neutron -> gris
            case 3: color = glm::vec3(1, 1, 0); break; // Electron -> amarillo
        }
        shaders->setVec3("color2", color);

        p->drawParticula();
    }
}

void Window::setUpCamera() {
    cameraPos = glm::vec3(0.0, 1.0, 5.0);
    cameraTarget = glm::vec3(0.0, 0.0, 0.0);
    cameraUp = glm::vec3(0.0, 1.0, 0.0);

    updateCamera();
}

void Window::updateCamera() {
    glm::vec3 center = cameraTarget;

    float x = radius * cos(pitch) * sin(yaw);
    float y = radius * sin(pitch);
    float z = radius * cos(pitch) * cos(yaw);

    cameraPos = center + glm::vec3(x, y, z);

    view = glm::lookAt(cameraPos, center, glm::vec3(0, 1, 0));

    float aspectRatio = (float)currentWidth / (float)currentHeight;
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 500.0f);
}

void Window::enviarUniforms() {
    glm::vec4 lightPosCam = view * glm::vec4(cameraPos, 1.0f); 
    shaders->setVec4("lightPosition", lightPosCam);

    shaders->setVec4("lightAmbient", glm::vec4(0.2f));
    shaders->setVec4("lightDiffuse", glm::vec4(0.8f));
    shaders->setVec4("lightSpecular", glm::vec4(1.0f));

    shaders->setVec4("matAmbient", glm::vec4(0.3f));
    shaders->setVec4("matDiffuse", glm::vec4(0.6f));
    shaders->setVec4("matSpecular", glm::vec4(0.8f));
    shaders->setFloat("matShininess", 32.0f);
}

void Window::inicializarShaders() {
    shaders = new Shaders("shaders/vertexShader.vert", "shaders/fragmentShader.frag");
    shaders->use();
}

void Window::calcularDelta() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Window::run() {
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
    updateViewport(currentWidth, currentHeight);

    setUpCamera();
    inicializarShaders();
    particulas.push_back(new Electron(glm::vec3(0,0,0)));
    particulas.push_back(new Proton(glm::vec3(2,0,0)));
    particulas.push_back(new Neutron(glm::vec3(4,0,0)));


    if (!Particulas::inicializado) {
        Particulas::initMesh();
    }

    while (!glfwWindowShouldClose(window)) {
        calcularDelta();
        
        glClearColor(0.1f, 0.05f, 0.1f, 1.0f); //Color de fondo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processHeldKeys();


        updateCamera();       
        shaders->use();
        enviarUniforms();
        drawObjects();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}
