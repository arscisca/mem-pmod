#ifndef MEM_PMOD_ELECTRICAL_H
#define MEM_PMOD_ELECTRICAL_H

#include "Types.hpp"

struct PULParameters {
    double Rsdc;
    double Rsac;
    double Ls;
    double Rpdc;
    double Rpac;
    double Lp;
    double Cp;
};

DataVector vectorize(const PULParameters &parameters);
PULParameters devectorize(const DataVector &data);

struct Parameters {
    double Rs;
    double Ls;
    double Rp;
    double Lp;
    double Cp;
};
#endif //MEM_PMOD_ELECTRICAL_H
