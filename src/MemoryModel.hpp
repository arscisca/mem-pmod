#ifndef MEM_PMOD_MEMORYMODEL_HPP
#define MEM_PMOD_MEMORYMODEL_HPP

#include <array>
#include <vector>
#include "Types.hpp"
#include "Electrical.h"
#include "TSection.h"

template <std::size_t NPowerPorts>
class GeometricMemoryModel {
public:
    GeometricMemoryModel() = default;
    GeometricMemoryModel(const std::array<double, NPowerPorts> &lengths) : _sections(lengths) {};

    void setSectionLengths(const std::array<double, NPowerPorts> &lengths) {
        for (size_t i = 0; i < NPowerPorts; i++)
            _sections[i].setLength(lengths[i]);
    }

    // --- Methods for electrical quantities ---
    Matrix2 groupABCD(size_t port1, size_t port2, double frequency, const PULParameters &pul_parameters) const {
        Matrix2 result = Matrix2::Identity();
        for (size_t i = port1; i < port2; i++) {
            Matrix2 section_ABCD = getSectionABCD(i, frequency, pul_parameters);
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
        return Matrix2{{y11, y12}, {y21, y22}};
    }

    Matrix2 computeZMatrix(size_t port1, size_t port2, double frequency, const PULParameters &pul_parameters) const {
        return computeYMatrix(port1, port2, frequency, pul_parameters).inverse();
    }

    // --- Operators ---
    const TSection &operator[](std::size_t i) const {
        return _sections[i];
    }

protected:
    virtual Matrix2 getSectionABCD(std::size_t section, double frequency, const PULParameters &pul_parameters) const {
        LumpedParameters parameters = _sections[section].computeParameters(frequency, pul_parameters);
        return _sections[section].computeABCD(frequency, parameters);
    }

private:
    std::array<GeometricTSection, NPowerPorts> _sections;
};

template<size_t NPowerPorts>
class MemoryModel : private GeometricMemoryModel<NPowerPorts> {
public:
    MemoryModel() : _sections(), _pul_parameters{NAN, NAN, NAN, NAN, NAN, NAN, NAN} {};
    MemoryModel(const std::array<double, NPowerPorts> lengths, const PULParameters &pul_parameters) :
            _sections(lengths),
            _pul_parameters(pul_parameters) {};

    const PULParameters &getPULParameters() const {
        return _pul_parameters;
    }

    void setPULParameters(const PULParameters &parameters, double fmax) {
        _pul_parameters = parameters;
        for (TSection &section : _sections) {
            section.setParameters(_pul_parameters, fmax);
        }
    }

    void setSectionLengths(const std::array<double, NPowerPorts> lengths) {
        for (std::size_t i = 0; i < NPowerPorts; i++) {
            _sections[i].setLength(lengths[i]);
        }
    }
    // --- Methods for electrical quantities

    Matrix2 groupABCD(size_t port1, size_t port2, double frequency) const {
        return GeometricMemoryModel<NPowerPorts>::groupABCD(port1, port2, frequency, _pul_parameters);
    }

    Matrix2 computeYMatrix(size_t port1, size_t port2, double frequency) const {
        return GeometricMemoryModel<NPowerPorts>::computeYMatrix(port1, port2, frequency, _pul_parameters);
    }

    Matrix2 computeZMatrix(size_t port1, size_t port2, double frequency) const {
        return GeometricMemoryModel<NPowerPorts>::computeZMatrix(port1, port2, frequency, _pul_parameters);
    }

    void printLogarithmicFrequencySweep(std::ostream &ostream,
                                        size_t port1, size_t port2,
                                        double fmin, double fmax,
                                        std::size_t fsamples) {
        double frequency = fmin;
        double frequency_factor = pow(fmax / fmin, 1.0 / (double) fsamples);
        // CSV header
        ostream << "f\tZ11\tZ12\tZ21\tZ22\n";
        ostream << std::scientific << std::showpos;
        // Print frequency sweep of Z matrix
        for (std::size_t i = 0; i < fsamples; i++) {
            Matrix2 Z = computeZMatrix(port1, port2, frequency);
            ostream << frequency << "\t" << Z(0, 0) << "\t" << Z(0, 1) << "\t" << Z(1, 0) << "\t" << Z(1, 1) << "\n";
            frequency *= frequency_factor;
        }
        ostream << std::flush;
    }

private:
    Matrix2 getSectionABCD(std::size_t section, double frequency, const PULParameters &) const {
        return _sections[section].ABCD(frequency);
    }

private:
    std::array<TSection, NPowerPorts> _sections;
    PULParameters _pul_parameters;
};

#endif //MEM_PMOD_MEMORYMODEL_HPP
