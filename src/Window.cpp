#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
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
    
}

void Window::run() {
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    while (!glfwWindowShouldClose(window)) {

        //Color por defecto negro
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}