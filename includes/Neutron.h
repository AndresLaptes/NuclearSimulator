#ifndef NEUTRON_H
#define NEUTRON_H
#include "Particulas.h"

class Neutron : public Particulas {
    public:
        Neutron(glm::vec3 p) : Particulas(double(1.67492749804e-27), double(0.8751e-15), double(0.0), p) {};
        int getTipo() const override;
};


#endif
