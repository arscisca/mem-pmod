#ifndef MEM_PMOD_MEMORYMODEL_HPP
#define MEM_PMOD_MEMORYMODEL_HPP

#include <array>
#include <vector>
#include "Types.hpp"
#include "Electrical.h"
#include "TSection.h"

template<size_t NPowerPorts>
class MemoryModel {
public:
    MemoryModel() = default;

    void setSectionLengths(const std::array<double, NPowerPorts> &lengths) {
        for (size_t i = 0; i < NPowerPorts; i++)
            _sections[i].setLength(lengths[i]);
    }

    const PULParameters &getPULParameters() const {
        return _pul_parameters;
    }

    void setPULParameters(const PULParameters &parameters) {
        _pul_parameters = parameters;
    }

    const TSection &operator[](size_t i) const {
        return _sections[i];
    }

    Matrix2 groupABCD(size_t port1, size_t port2, double frequency, const PULParameters &pul_parameters) const {
        Matrix2 result = Matrix2::Identity();
        for (size_t i = port1; i < port2; i++) {
            Matrix2 section_ABCD = _sections[i].computeABCD(frequency, pul_parameters);
            result = result * section_ABCD;
        }
        return result;
    }

    Matrix2 computeYMatrix(size_t port1, size_t port2, double frequency, const PULParameters &pul_parameters) const {
        // Divide power bus in three sections and compute the group ABCD matrices
        Matrix2 m1 = groupABCD(0, port1, frequency, pul_parameters);
        Matrix2 m2 = groupABCD(port1, port2, frequency, pul_parameters);
        Matrix2 m3 = groupABCD(port2, NPowerPorts - 1, frequency, pul_parameters);
        // Compute Y matrix between the two ports
        Complex y11 = m1(1, 0) / m1(1, 1) + m2(1, 1) / m2(0, 1);
        Complex y12 = - 1.0 / m2(0, 1);
        Complex y21 = y12;
        Complex y22 = m2(0, 0) / m2(0, 1) + m3(1, 0) / m3(0, 0);
        return Matrix2({{y11, y12}, {y21, y22}});
    }

    Matrix2 computeZMatrix(size_t port1, size_t port2, double frequency, const PULParameters &pul_parameters) const {
        return computeYMatrix(port1, port2, frequency, pul_parameters).inverse();
    }

    Matrix2 computeZMatrix(size_t port1, size_t port2, double frequency) const {
        return computeZMatrix(port1, port2, frequency, _pul_parameters);
    }

    void printLogarithmicFrequencySweep(std::ostream &ostream,
                                        size_t port1, size_t port2,
                                        double fmin, double fmax,
                                        std::size_t fsamples) {
        double frequency = fmin;
        double frequency_factor = pow(fmax / fmin, 1.0 / fsamples);
        // CSV header
        ostream << "f,Z11_real,Z11_imag,Z12_real,Z12_imag,Z21_real,Z21_imag,Z22_real,Z21_imag\n";
        ostream << std::scientific;
        // Print frequency sweep of Z matrix
        for (std::size_t i = 0; i < fsamples; i++) {
            Matrix2 Z = computeZMatrix(port1, port2, frequency);
            ostream << frequency << "," << complexMatrixToCSV(Z) << "\n";
            frequency *= frequency_factor;
        }
        ostream << std::flush;
    }
private:
    std::array<TSection, NPowerPorts> _sections;
    PULParameters _pul_parameters;
};

#endif //MEM_PMOD_MEMORYMODEL_HPP
