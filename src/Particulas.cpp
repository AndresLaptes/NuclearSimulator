#include "Particulas.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace std;

GLuint Particulas::VAO = 0;
GLuint Particulas::VBO = 0;
GLuint Particulas::EBO = 0;
int Particulas::indexCount = 0;
bool Particulas::inicializado = false;


void Particulas::initMesh() {
    const int sectorCount = 36; //aumentar esto aumenta calidad esferas
    const int stackCount = 18;

    std::vector<float> vertices;   // Posición (x,y,z) + Normal (nx,ny,nz)
    std::vector<unsigned int> indices;

    float x, y, z, xy;
    float nx, ny, nz;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = cosf(stackAngle);
        z = sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            // Posición
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normal
            nx = x;
            ny = y;
            nz = z;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    indexCount = static_cast<int>(indices.size());

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO: posición + normal = 6 floats
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Atributo 0: posición
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    inicializado = true;
}

float Particulas::tipoRadioEscena() const {
    int tipo = getTipo();
    if (tipo == 1) return ESCENA_PROTON;
    if (tipo == 2) return ESCENA_NEUTRON;
    if (tipo == 3) return ESCENA_ELECTRON;
    return 0;
}

void Particulas::cleanUp() {
    if (inicializado) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        inicializado = false;
    }
}

void Particulas::drawParticula() {
    glBindVertexArray(VAO);  // esfera compartida
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

double Particulas::getMasa() const {
    return masa;
}

double Particulas::getRadio() const {
    return radio;
}

double Particulas::getCarga() const {
    return carga;
}

glm::vec3 Particulas::getPosicion() const { 
    return posicion; 
}

void Particulas::setPosicion(glm::vec3 p) {
    posicion = p;
}

void Particulas::updatePosicion(glm::vec3 p) {
    posicion += p;
}

glm::mat4 Particulas::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, posicion);
    model = glm::scale(model, glm::vec3(tipoRadioEscena())); 
    return model;
}