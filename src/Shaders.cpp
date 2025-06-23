#include "Shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shaders::Shaders(const char* vertexPath, const char* fragmentPath) {
    std::ifstream vFile(vertexPath), fFile(fragmentPath);
    std::stringstream vStream, fStream;

    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vCode = vStream.str();
    std::string fCode = fStream.str();

    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();

    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shaders::use() const {
    glUseProgram(ID);
}

void Shaders::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shaders::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shaders::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shaders::setVec4(const std::string &name, const glm::vec3 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shaders::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shaders::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}