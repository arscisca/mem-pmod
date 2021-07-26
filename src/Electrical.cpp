#include "Electrical.h"

std::ostream &operator<<(std::ostream &ostream, const PULParameters &pul_parameters) {
    ostream << pul_parameters.Rsdc  << "\n"
            << pul_parameters.Rsac  << "\n"
            << pul_parameters.Ls    << "\n"
            << pul_parameters.Rpdc  << "\n"
            << pul_parameters.Rpac  << "\n"
            << pul_parameters.Lp    << "\n"
            << pul_parameters.Cp;
    return ostream;
}

std::istream &operator>>(std::istream &istream, PULParameters &pul_parameters) {
    istream >> pul_parameters.Rsdc >> pul_parameters.Rsac >> pul_parameters.Ls
            >> pul_parameters.Rpdc >> pul_parameters.Rpac >> pul_parameters.Lp >> pul_parameters.Cp;
    return istream;
}

std::ostream &operator<<(std::ostream &ostream, const LumpedParameters &lumped_parameters) {
    ostream << lumped_parameters.Rs << "\n"
            << lumped_parameters.Ls << "\n"
            << lumped_parameters.Rp << "\n"
            << lumped_parameters.Lp << "\n"
            << lumped_parameters.Cp << "\n";
    return ostream;
}

std::istream &operator>>(std::istream &istream, LumpedParameters &lumped_parameters) {
    istream >> lumped_parameters.Rs >> lumped_parameters.Ls
            >> lumped_parameters.Rp >> lumped_parameters.Lp >> lumped_parameters.Cp;
    return istream;
}

#ifdef PMOD_TEST
bool operator==(const PULParameters &p1, const PULParameters &p2) {
    return  p1.Rsdc == p2.Rsdc &&
            p1.Rsac == p2.Rsac &&
            p1.Ls == p2.Ls &&
            p1.Rpdc == p2.Rpdc &&
            p1.Rpac == p2.Rpac &&
            p1.Lp == p2.Lp &&
            p1.Cp == p2.Cp;
}

bool operator==(const LumpedParameters &p1, const LumpedParameters &p2) {
    return  p1.Rs == p2.Rs &&
            p1.Ls == p2.Ls &&
            p1.Rp == p2.Rp &&
            p1.Lp == p2.Lp &&
            p1.Cp == p2.Cp;
}
#endif