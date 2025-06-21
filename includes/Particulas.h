#ifndef PARTICULAS_H
#define PARTICULAS_H
#include <glad/glad.h>
#include <glm/glm.hpp>

class Particulas {
    protected:
        double masa; //Kg
        double radio; //m
        double carga; //Colombus
        
        glm::vec3 posicion;
        Particulas(double m, double r, double c, glm::vec3 p) : masa(m), radio(r), carga(c), posicion(p) {}

    public:
        virtual ~Particulas() = default;
        virtual int getTipo() const = 0;

        double getMasa() const;
        double getRadio() const ;
        double getCarga() const;
        glm::vec3 getPosicion() const;

        void setPosicion(glm::vec3 p);
        void updatePosicion(glm::vec3 p); //actual + p
};


#endif