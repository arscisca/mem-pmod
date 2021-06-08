#include "TSection.h"

// --- GeometricTSection ---
GeometricTSection::GeometricTSection() = default;
GeometricTSection::GeometricTSection(double length) : _length(length) {}
GeometricTSection::GeometricTSection(const GeometricTSection &copy) = default;

double GeometricTSection::getLength() const {
    return _length;
}

void GeometricTSection::setLength(double length) {
    _length = length;
}

LumpedParameters GeometricTSection::computeParameters(double frequency, const PULParameters &pul_parameters) const {
    double omega = (2 * M_PI) * frequency;
    double Rs = _length * std::max({pul_parameters.Rsdc, pul_parameters.Rsac * sqrt(omega)});
    double Ls = _length * pul_parameters.Ls;
    double Rp = 1.0 / _length * std::max({pul_parameters.Rpdc, pul_parameters.Rpac * sqrt(omega)});
    double Lp = 1.0 / _length * pul_parameters.Lp;
    double Cp = _length * pul_parameters.Cp;
    return {Rs, Ls, Rp, Lp, Cp};
}

Matrix2 GeometricTSection::computeABCD(double frequency, const LumpedParameters &parameters) const {
    double omega = (2 * M_PI) * frequency;
    constexpr Complex j = Complex(0.0, 1.0);
    Complex z =  j * omega * parameters.Ls + parameters.Rs;
    Complex y = (j * omega * parameters.Cp) /
                (1.0 + j * omega * parameters.Rp * parameters.Cp - omega * omega * parameters.Lp * parameters.Cp);
    return Matrix2 {
            {1.0 + z * y, (z * y + 2.0) * z},
            {y,           1.0 + z * y}
    };
}

// --- TSection ---
TSection::TSection() = default;
TSection::TSection(double length) : GeometricTSection(length), _parameters() {}
TSection::TSection(const GeometricTSection &geometric) : GeometricTSection(geometric), _parameters() {}

double TSection::getLength() const {
    return _length;
}

void TSection::setLength(double length) {
    _length = length;
}

void TSection::setParameters(const LumpedParameters &parameters) {
    _parameters = parameters;
}

void TSection::setParameters(const PULParameters &pul_parameters, double fmax) {
    setParameters(GeometricTSection::computeParameters(fmax, pul_parameters));
}

const LumpedParameters & TSection::getParameters() const {
    return _parameters;
}

Matrix2 TSection::ABCD(double frequency) const {
    return GeometricTSection::computeABCD(frequency, _parameters);
}
