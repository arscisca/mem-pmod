#include "Electrical.h"

#ifdef PMOD_TEST
bool operator==(const Parameters &p1, const Parameters &p2) {
    return  p1.Rs == p2.Rs &&
            p1.Ls == p2.Ls &&
            p1.Rp == p2.Rp &&
            p1.Lp == p2.Lp &&
            p1.Cp == p2.Cp;
}

std::ostream &operator<<(std::ostream &ostream, const Parameters &parameters) {
    ostream << "{Rs=" << parameters.Rs << ", "
            << "Ls=" << parameters.Ls << ", "
            << "Rp=" << parameters.Rp << ", "
            << "Lp=" << parameters.Lp << ", "
            << "Cp=" << parameters.Cp << "}";
    return ostream;
}
#endif