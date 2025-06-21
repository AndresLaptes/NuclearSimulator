#ifndef PROTON_H
#define PROTON_H
#include "Particulas.h"

class Proton : public Particulas {
    private:

    public:
        Proton(glm::vec3 p) : Particulas(double(1.67262192369e-27), double(0.84e-15), double(1.6021766340e-19), p) {};
        int getTipo() const override;

};

#endif