#ifndef WINDOW_H
#define WINDOW_H
#include <GLFW/glfw3.h>
#include <i386-linux-gnu/sys/types.h>
#include <string>
using namespace std;

class Window {
    private:
        GLFWwindow* window;
        bool fullscrean;
        int windowPosX, windowPosY, windowWidth, windowHeight;
        

        void Fullscrean();
    public:
        Window(uint width, uint height, const char* title);
        
        void run();
        void endWindow();
};


#endif