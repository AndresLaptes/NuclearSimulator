#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Proton.h"
using namespace std;

Window::Window(uint width, uint height) {
    if (!glfwInit()) cout << "Error" << endl; //Error

    fullscrean = false;
    window = glfwCreateWindow(width, height, "Nuclear Simulator", nullptr, nullptr);

    if (!window) cout << "Error crear window" << endl;

    glfwGetWindowPos(window, &windowPosX, &windowPosY);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallback);
    inicializarMapa();
}

void Window::endWindow() {
    glfwTerminate();
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
    if (action != GLFW_PRESS) return;

    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
    auto it = self->keyActions.find(key);
    if (it != self->keyActions.end()) {
        it->second();  // Llama a la lambda funcion
    }
}

void Window::inicializarMapa() {
    keyActions[GLFW_KEY_F11] = [this]() {
        Fullscrean();
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
            case 2: color = glm::vec3(0, 1, 0); break; // Neutron -> verde
            case 3: color = glm::vec3(0, 0, 1); break; // Electron -> azul
        }
        shaders->setVec3("color", color);

        p->drawParticula();
    }
}

void Window::setUpCamera() {
    cameraPos = glm::vec3(0.0, 0.0, 5.0);
    cameraTarget = glm::vec3(0.0, 0.0, 0.0);
    cameraUp = glm::vec3(0.0, 1.0, 0.0);

    updateCamera();
}

void Window::updateCamera() {
    view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
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
    shaders = new Shaders("../shaders/vertexShader.glsl", "../shaders/fragmentShader.glsl");
    shaders->use();
}

void Window::run() {
    particulas.push_back(new Proton(glm::vec3(0,0,0)));
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    setUpCamera();
    inicializarShaders();

    while (!glfwWindowShouldClose(window)) {
        //Color por defecto de fondo negro
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        updateCamera();       
        shaders->use();
        enviarUniforms();
        drawObjects();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}