#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <i386-linux-gnu/sys/types.h>
#include <string>
#include <unordered_map>
#include <functional>
using namespace std;

class Window {
    private:
        GLFWwindow* window;
        bool fullscrean;
        int windowPosX, windowPosY, windowWidth, windowHeight;
           
        unordered_map<int, function<void()>> keyActions;

        void Fullscrean();
        
        //Interacciones la pantalla
        void inicializarMapa();
        void drawObjects();
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    public:
        Window(uint width, uint height);
        
        void run();
        void endWindow();
};


#endif