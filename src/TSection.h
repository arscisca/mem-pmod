
#ifndef MEM_PMOD_TSECTION_H
#define MEM_PMOD_TSECTION_H

#include "Types.hpp"
#include "Electrical.h"

class GeometricTSection {
public:
    GeometricTSection();
    explicit GeometricTSection(double length);
    GeometricTSection(const GeometricTSection &copy);

    double getLength() const;
    void setLength(double length);

    LumpedParameters computeParameters(double frequency, const PULParameters &pul_parameters) const;
    Matrix2 computeABCD(double frequency, const LumpedParameters &parameters) const;

protected:
    double _length;
};

class TSection : private GeometricTSection {
public:
    TSection();
    explicit TSection(double length);
    explicit TSection(const GeometricTSection &geometric);

    double getLength() const;
    void setLength(double length);

    const LumpedParameters &getParameters() const;
    void setParameters(const LumpedParameters &parameters);
    void setParameters(const PULParameters &pul_parameters, double fmax);

    Matrix2 ABCD(double frequency) const;
private:
    LumpedParameters _parameters;
};


#endif //MEM_PMOD_TSECTION_H
