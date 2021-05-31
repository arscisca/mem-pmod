#include "TSection.h"

TSection::TSection() = default;

TSection::TSection(double length) : _length(length), _parameters() {}

double TSection::getLength() const {
    return _length;
}

void TSection::setLength(double length) {
    _length = length;
}

void TSection::setParameters(const Parameters &parameters) {
    _parameters = parameters;
}

const Parameters & TSection::getParameters() const {
    return _parameters;
}

Parameters TSection::computeParameters(double frequency, const PULParameters &pul_parameters) const {
    double omega = (2 * M_PI) * frequency;
    double Rs = _length * std::max({pul_parameters.Rsdc, pul_parameters.Rsac * sqrt(omega)});
    double Ls = _length * pul_parameters.Ls;
    double Rp = 1.0 / _length * std::max({pul_parameters.Rpdc, pul_parameters.Rpac * sqrt(omega)});
    double Lp = 1.0 / _length * pul_parameters.Lp;
    double Cp = _length * pul_parameters.Cp;
    return {Rs, Ls, Rp, Lp, Cp};
}

Matrix2 TSection::computeABCD(double frequency, const PULParameters &pul_parameters) const {
    double omega = (2 * M_PI) * frequency;
    Parameters parameters = computeParameters(frequency, pul_parameters);
    constexpr Complex j = Complex(0.0, 1.0);
    Complex z =  j * omega * parameters.Ls + parameters.Rs;
    Complex y = (j * omega * parameters.Cp) /
                (1.0 + j * omega * parameters.Rp * parameters.Cp - omega * omega * parameters.Lp * parameters.Cp);
    return Matrix2({
        {1.0 + z * y,   (z * y + 2.0) * z},
        {          y,         1.0 + z * y}
    });
}