#ifndef SHADERS_H
#define SHADERS_H
#include <glad/glad.h>
#include <glm/glm.hpp> 
#include <string>
using namespace std;

class Shaders {
    private:
        GLuint program;
        string loadShader(const string& filename);
        GLuint copilarShader(GLenum type, const string& source);
    
    public:

    Shaders(const string& vsFile, const string& fsFile); 
    void use() const;  

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;

};

#endif 