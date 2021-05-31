#ifndef MEM_PMOD_ELECTRICAL_H
#define MEM_PMOD_ELECTRICAL_H

#include <string>
#include <sstream>
#include "Types.hpp"

std::string complexMatrixToCSV(const Matrix2 &m);

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

// Testing helpers
#ifdef PMOD_TEST
#include <iostream>
bool operator==(const Parameters &p1, const Parameters &p2);
std::ostream &operator<<(std::ostream &ostream, const Parameters &parameters);
#endif

#endif //MEM_PMOD_ELECTRICAL_H
