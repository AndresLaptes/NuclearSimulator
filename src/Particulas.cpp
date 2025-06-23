#include "Particulas.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
using namespace std;

unsigned int Particulas::VAO = 0;
unsigned int Particulas::VBO = 0;
bool Particulas::inicializado = false;

void Particulas::initMesh() {
    const int sectorCount = 36;
    const int stackCount = 18;
    
    vector <float> vertices;
    float x,y,z, xy;
    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;        
        xy = cosf(stackAngle);             
        z = sinf(stackAngle);               

        for(int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = cosf(sectorAngle) * xy;
            y = sinf(sectorAngle) * xy;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    inicializado = true;
}

float Particulas::tipoRadioEscena() {
    int tipo = getTipo();
    if (tipo == 1) return ESCENA_PROTON;
    if (tipo == 2) return ESCENA_NEUTRON;
    if (tipo == 3) return ESCENA_ELECTRON;
}

void Particulas::drawParticula(Shaders& shaders) {
    
    
    glm::mat4 model = glm::translate(glm::mat4(1.0f), posicion);
    model = glm::scale(model, glm::vec3());
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

