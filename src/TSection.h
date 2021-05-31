
#ifndef MEM_PMOD_TSECTION_H
#define MEM_PMOD_TSECTION_H

#include "Types.hpp"
#include "Electrical.h"

class TSection {
public:
    TSection();
    explicit TSection(double length);

    double getLength() const;
    void setLength(double length);

    void setParameters(const Parameters &parameters);
    const Parameters &getParameters() const;
    Parameters computeParameters(double frequency, const PULParameters &pul_parameters) const;

    Matrix2 computeABCD(double frequency, const PULParameters &pul_parameters) const;
private:
    double _length;
    Parameters _parameters;
};


#endif //MEM_PMOD_TSECTION_H
