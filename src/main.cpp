#include <iostream>
#include "Window.h"
#include <GLFW/glfw3.h>
using namespace std;

int main() {
    Window window(800, 600);

    window.run();
    
    window.endWindow();
    return 0;
} 