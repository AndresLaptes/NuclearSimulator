#ifndef SHADERS_H
#define SHADERS_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shaders {
    public:

    unsigned int ID;

    Shaders(const char* vertexPath, const char* fragmentPath); // Constructor que carga shaders desde archivos
    void use() const;  // Activa este shader

    // Funciones utilitarias para pasar datos uniform a los shaders
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

#endif 