#ifndef MEM_PMOD_ELECTRICAL_H
#define MEM_PMOD_ELECTRICAL_H

#include <iostream>
#include <cmath>

struct PULParameters {
    double Rsdc;
    double Rsac;
    double Ls;
    double Rpdc;
    double Rpac;
    double Lp;
    double Cp;
};

constexpr PULParameters reference_pul_parameters{
        1000.0,
        31.6e-3, // 1000 / sqrt(1e9)
        1.0e-3,
        31.6e-9, // 1.0e-3 / sqrt(1.0e9),
        0.0,
        10.0e-12 / 1000.0,
        1.0e-9 * 1000.0
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
bool operator==(const PULParameters &p1, const PULParameters &p2);
bool operator==(const LumpedParameters &p1, const LumpedParameters &p2);
#endif

#endif //MEM_PMOD_ELECTRICAL_H
