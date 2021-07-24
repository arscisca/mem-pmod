#ifndef MEM_PMOD_ELECTRICAL_H
#define MEM_PMOD_ELECTRICAL_H

#include <iostream>

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

std::ostream &operator<<(std::ostream &ostream, const PULParameters &pul_parameters);
std::istream &operator>>(std::istream &istream, PULParameters &pul_parameters);

std::ostream &operator<<(std::ostream &ostream, const LumpedParameters &lumped_parameters);
std::istream &operator>>(std::istream &istream, LumpedParameters &lumped_parameters);

// Testing helpers
#ifdef PMOD_TEST
#include <iostream>
#include <string>
#include <sstream>
bool operator==(const LumpedParameters &p1, const LumpedParameters &p2);
#endif

#endif //MEM_PMOD_ELECTRICAL_H
