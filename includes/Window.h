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
           
        unordered_map<int, function<void()>> keyActions;

        void Fullscrean();
        
        //Camera
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
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        //Objetos
        vector<Particulas*> particulas;

        //TEMPORAL 
        GLuint VAO, VBO, EBO;
    public:
        Window(uint width, uint height);
        
        void run();
        void endWindow();
};


#endif