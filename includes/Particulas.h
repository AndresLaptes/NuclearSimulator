#ifndef PARTICULAS_H
#define PARTICULAS_H
#include "Shaders.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Particulas {
    private:
        float tipoRadioEscena() const;

    protected:
        const float ESCENA_PROTON = 1.0f;
        const float ESCENA_NEUTRON = 1.035f;
        const float ESCENA_ELECTRON = 0.001f;

        double masa; //Kg
        double radio; //m
        double carga; //Colombus
        
        static GLuint VAO, VBO, EBO; // Esfera compartida entre todas
        static int indexCount;

        glm::vec3 posicion;
        Particulas(double m, double r, double c, glm::vec3 p) : masa(m), radio(r), carga(c), posicion(p) {
            if (not inicializado) initMesh();
        }

    public:
        virtual ~Particulas() = default;
        virtual int getTipo() const = 0;
        virtual void setPosicion(glm::vec3 p);
        virtual void updatePosicion(glm::vec3 p); //actual + p
        virtual void drawParticula();
        virtual void cleanUp();

        virtual double getMasa() const;
        virtual double getRadio() const ;
        virtual double getCarga() const;
        glm::vec3 getPosicion() const;
        
        glm::mat4 getModelMatrix() const;

        static bool inicializado;
        static void initMesh(); //Inicializar los VAO's

};


#endif