#include <iostream>
using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>
#include <vector>

string loadShaderSource(const string& filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(GLenum type, const string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        cerr << "Error al compilar shader: " << info << endl;
    }
    return shader;
}

GLuint createShaderProgram(const string& vsFile, const string& fsFile) {
    string vsSource = loadShaderSource(vsFile);
    string fsSource = loadShaderSource(fsFile);

    GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main() {
    if (!glfwInit()) {
        cerr << "Error al inicializar GLFW" << endl;
        return -1;
    }

    // Evita que GLFW incluya gl.h
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLuint width = 800;
    GLuint height = 600;
    GLFWwindow* window = glfwCreateWindow(width, height, "Test", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cerr << "Error al inicializar GLAD" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, width, height);


    GLuint shaderProgram = createShaderProgram("testV.vert", "testF.frag");
                            //VERT      //COLOR
    vector<float> vertices = {0, 1, 0,  1, 0, 0,
                              0, 0, 0,  1, 0, 0,
                              1, 1, 0,  1, 0, 0,
                              1, 0, 0,  1, 0, 0};

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    glm::mat4 model = glm::mat4(1.0f);

    //Camara
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),  // posición de la cámara
        glm::vec3(0.0f, 0.0f, 0.0f),  // hacia dónde mira
        glm::vec3(0.0f, 1.0f, 0.0f)   // vector "arriba"
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),          // campo de visión
        (float)width / (float)height, // aspecto
        0.1f,                         // near plane
        100.0f                        // far plane
    );

    glm::mat4 mvp = projection * view * model;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        GLint mvpLoc = glGetUniformLocation(shaderProgram, "modelViewProjectionMatrix");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, &mvp[0][0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}