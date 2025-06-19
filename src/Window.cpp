#include "Window.h"
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

Window::Window(uint width, uint height, const char* title) {
    if (!glfwInit()) cout << "Error" << endl; //Error

    fullscrean = false;
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) cout << "Error crear window" << endl;

    glfwGetWindowPos(window, &windowPosX, &windowPosY);
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
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

void Window::run() {
    
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        
        if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) {
            Fullscrean();
            // Esperar hasta que se libere la tecla para evitar múltiples llamadas
            while (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
                glfwPollEvents();
        }

        
        //Color por defecto negro
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
}