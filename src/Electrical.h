#ifndef MEM_PMOD_ELECTRICAL_H
#define MEM_PMOD_ELECTRICAL_H

struct PULParameters {
    double Rsdc;
    double Rsac;
    double Ls;
    double Rpdc;
    double Rpac;
    double Lp;
    double Cp;
};

struct LumpedParameters {
    double Rs;
    double Ls;
    double Rp;
    double Lp;
    double Cp;
};

// Testing helpers
#ifdef PMOD_TEST
#include <iostream>
#include <string>
#include <sstream>
bool operator==(const LumpedParameters &p1, const LumpedParameters &p2);
std::ostream &operator<<(std::ostream &ostream, const LumpedParameters &parameters);
#endif

#endif //MEM_PMOD_ELECTRICAL_H
