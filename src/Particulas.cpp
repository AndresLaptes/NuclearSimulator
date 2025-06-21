#include "Particulas.h"

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