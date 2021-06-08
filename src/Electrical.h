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
#include <string>
#include <sstream>
bool operator==(const Parameters &p1, const Parameters &p2);
std::ostream &operator<<(std::ostream &ostream, const Parameters &parameters);
#endif

#endif //MEM_PMOD_ELECTRICAL_H
