
#ifndef MEM_PMOD_TSECTION_H
#define MEM_PMOD_TSECTION_H

#include "Types.hpp"
#include "Electrical.h"

class TSection {
public:
    TSection();

    double getLength() const;
    void setLength(double length);

    Parameters computeParameters(double frequency, const PULParameters &pul_parameters);

    Matrix3 computeABCD(double frequency, const PULParameters &pul_parameters);
private:
    double _length;
};


#endif //MEM_PMOD_TSECTION_H
