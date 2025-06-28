#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <i386-linux-gnu/sys/types.h>
#include <string>
#include "Shaders.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <functional>
#include <vector>
#include "Particulas.h"
using namespace std;

class Window {
    private:
        GLFWwindow* window;
        bool fullscrean;
        int windowPosX, windowPosY, windowWidth, windowHeight;
        int currentWidth, currentHeight;

        unordered_map<int, std::function<void()>> oneTimeKeyActions;
        unordered_map<int, function<void()>> keyActions;
        unordered_map<int, bool> keysPressed;
        
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        void calcularDelta();

        void Fullscrean();
        void processHeldKeys();

        //Movimientos Camera
        bool rightMousePressed;
        double lastMouseX, lastMouseY;
        float yaw, pitch;
        float radius;
        static void mouseButtonCallback(GLFWwindow* win, int button, int action, int mods);
        static void mouseCallback(GLFWwindow* win, double xpos, double ypos);
        void move(glm::vec3 dir);
        void velocidadCamera(float p); //Temporal

        //Camera
        float velocidad;
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec3 cameraPos;
        glm::vec3 cameraTarget;
        glm::vec3 cameraUp;


        //Shaders
        Shaders* shaders;
        void inicializarShaders();
        void enviarUniforms();

        //Interacciones la pantalla
        void inicializarMapa();
        void drawObjects();
        void setUpCamera();
        void updateCamera();
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);
        void updateViewport(int width, int height);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        //Objetos
        vector<Particulas*> particulas;
    public:
        Window(uint width, uint height);
        
        void run();
        void endWindow();
};


#endif