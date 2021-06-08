#include "Electrical.h"

#ifdef PMOD_TEST
bool operator==(const LumpedParameters &p1, const LumpedParameters &p2) {
    return  p1.Rs == p2.Rs &&
            p1.Ls == p2.Ls &&
            p1.Rp == p2.Rp &&
            p1.Lp == p2.Lp &&
            p1.Cp == p2.Cp;
}

std::ostream &operator<<(std::ostream &ostream, const LumpedParameters &parameters) {
    ostream << "{Rs=" << parameters.Rs << ", "
            << "Ls=" << parameters.Ls << ", "
            << "Rp=" << parameters.Rp << ", "
            << "Lp=" << parameters.Lp << ", "
            << "Cp=" << parameters.Cp << "}";
    return ostream;
}
#endif