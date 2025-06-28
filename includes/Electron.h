#ifndef ELECTRON_H
#define ELECTRON_H
#include "Particulas.h"


class Electron : public Particulas {
    public:
        Electron(glm::vec3 p) : Particulas(double(9.1093837015e-31), double(1.0e-18), double(-1.6021766340e-19), p) {};
        int getTipo() const override;
};

#endif