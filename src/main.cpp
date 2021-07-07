#include <iostream>
#include "MemoryModel.hpp"
#include "Measurements.h"

int main(int argc, char **argv) {
    const unsigned nsections = 23;
    MemoryModel<nsections> memory;
    Measurements measurements = readMeasurements("../data/measurements.txt", 2, 20);
    // Initialize eigen with multithreading
    Eigen::initParallel();
    /* PULParameters pul_parameters;
    // Initialize paper with the results presented on the paper
    const double total_length = 4074.0e-6;
    std::array<double, nsections> lengths;
    lengths.fill(total_length / nsections);
    memory.setSectionLengths(lengths);
    pul_parameters.Rsdc = 1211.0;
    pul_parameters.Rsac = 708.0 / sqrt(1.0e9);
    pul_parameters.Rpdc = 2.048e-3;
    pul_parameters.Rpac = 0.882e-3 / sqrt(1.0e9);
    pul_parameters.Ls = 0.0;
    pul_parameters.Lp = 6.908e-12 / 1000.0;
    pul_parameters.Cp = 1.342e-9 * 1000.0;
    memory.setPULParameters(pul_parameters, 10.0e9);

    // Print frequency sweep
    memory.printLogarithmicFrequencySweep(std::cout, 2, 20, 1.0e6, 10.0e9, 200);
    */
    const double total_length = 4074.0e-6;
    std::array<double, nsections> lengths;
    lengths.fill(total_length / nsections);
    memory = MemoryModel<nsections>::fit(measurements, lengths, pmod::optimization::CDESCENT);
    std::cout << memory;
    memory.printLogarithmicFrequencySweep(std::cout, 2, 20, 1.0e6, 10.0e9, 200);
    return 0;
}
