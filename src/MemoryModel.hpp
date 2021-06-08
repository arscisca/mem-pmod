#ifndef MEM_PMOD_MEMORYMODEL_HPP
#define MEM_PMOD_MEMORYMODEL_HPP

#include <array>
#include <vector>
#include "Types.hpp"
#include "Electrical.h"
#include "Measurements.h"
#include "TSection.h"
#include "Optimization.hpp"

template <std::size_t NPowerPorts>
class GeometricMemoryModel {
public:
    GeometricMemoryModel() = default;
    GeometricMemoryModel(const std::array<double, NPowerPorts> &lengths) {
        for (size_t i = 0; i < NPowerPorts; i++) {
            _sections[i].setLength(lengths[i]);
        }
    };

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
    MemoryModel(const std::array<double, NPowerPorts> lengths, const PULParameters &pul_parameters, double fmax) {
        for (std::size_t i = 0; i < NPowerPorts; i++)
            _sections[i].setLength(lengths[i]);
        setPULParameters(pul_parameters, fmax);
    };

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
        ostream << "f Z11 Z12 Z21 Z22\n";
        ostream << std::scientific << std::showpos;
        // Print frequency sweep of Z matrix
        for (std::size_t i = 0; i < fsamples; i++) {
            Matrix2 Z = computeZMatrix(port1, port2, frequency);
            ostream << frequency << " " << Z(0, 0) << " " << Z(0, 1) << " " << Z(1, 0) << " " << Z(1, 1) << "\n";
            frequency *= frequency_factor;
        }
        ostream << std::flush;
    }

    // --- Fitting ---
    /**
     * Fit measured data and generate a MemoryModel through a specific algorithm
     * @param measurements measured Z matrix
     * @param lengths section lengths of the physical memory
     * @param method optimization method
     * @return an initialized MemoryModel with the fitted parameters
     */
    static MemoryModel fit(const Measurements &measurements,
                           const std::array<double, NPowerPorts> lengths,
                           pmod::optimization::Algorithm method) {
        // Create geometric model
        GeometricMemoryModel<NPowerPorts> geometric_model(lengths);
        // Run optimization function to get PUL Parameters
        Vector<7> initial_values = {
                1211.0,
                708.0 / sqrt(1.0e9),
                2.048e-3,
                0.882e-3 / sqrt(1.0e9),
                0.0,
                6.908e-12 / 1000.0,
                1.342e-9 * 1000.0
        };
        Vector<7> vectorized_pul_parameters = pmod::optimization::optimize<7>(
                pmod::optimization::POWELL,
                [&geometric_model, &measurements](const Vector<7> &vectorized_pul_parameters) {
                    PULParameters pul_parameters = devectorizeFittingInput(vectorized_pul_parameters);
                    return fittingError(geometric_model, pul_parameters, measurements);
                },
                initial_values,
                1.0e-5);
        PULParameters pul_parameters = devectorizeFittingInput(vectorized_pul_parameters);
        // Create complete memory model
        MemoryModel<NPowerPorts> model(lengths,
                                       pul_parameters,
                                       measurements.frequencies[measurements.frequencies.size() - 1]);
        return model;
    }
private:
    Matrix2 getSectionABCD(std::size_t section, double frequency, const PULParameters &) const {
        return _sections[section].ABCD(frequency);
    }

    // --- Fitting ---
    /**
     * @brief convert a Vector<7> into a PULParameters struct, useful for fitting
     * @param vector input vector
     * @return PULParameters struct
     */
    static PULParameters devectorizeFittingInput(const Vector<7> &vector) {
        return {vector[0], vector[1], vector[2], vector[3], vector[4], vector[5], vector[6]};
    }

    /**
     * @brief compute the fitting error by comparing a model to reference measurements
     * @param model memory geometrical model
     * @param pul_parameters fitting target, the current under-test value for the PUL parameters
     * @param measurements reference measurements
     * @return fitting error computed as the sum along all the measured frequencies of the squared norm of (Z_measured
     * - Z_computed)
     */
    static double fittingError(
            const GeometricMemoryModel<NPowerPorts> &model,
            const PULParameters &pul_parameters,
            const Measurements &measurements) {
        const std::vector<double> frequencies = measurements.frequencies;
        double error = 0.0;
        for (std::size_t i = 0; i < frequencies.size(); i++) {
            double frequency = frequencies[i];
            Matrix2 measured_Z = measurements.Z[i];
            Matrix2 computed_Z = model.computeZMatrix(measurements.port1, measurements.port2,
                                                      frequency,
                                                      pul_parameters);
            double sqnorm = (measured_Z - computed_Z).squaredNorm();
            error += sqnorm;
        }
        return error;
    }

private:
    std::array<TSection, NPowerPorts> _sections;
    PULParameters _pul_parameters;
};

#endif //MEM_PMOD_MEMORYMODEL_HPP
