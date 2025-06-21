#include <iostream>
#include "Window.h"
using namespace std;

int main() {
    Window window(800, 600);

    window.run();
    
    window.endWindow();
    return 0;
} 