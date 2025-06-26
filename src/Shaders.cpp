#include "Shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

string Shaders::loadShader(const std::string& filename) {
    ifstream file(filename);
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shaders::copilarShader(GLenum type, const string& source) {
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


Shaders::Shaders(const string& vsFile, const string& fsFile) {
    string vsSource = loadShader(vsFile);
    string fsSource = loadShader(fsFile);

    GLuint vs = copilarShader(GL_VERTEX_SHADER, vsSource);
    GLuint fs = copilarShader(GL_FRAGMENT_SHADER, fsSource);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shaders::use() const {
    glUseProgram(program);
}

void Shaders::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}
void Shaders::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}
void Shaders::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}
void Shaders::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
void Shaders::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shaders::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shaders::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}