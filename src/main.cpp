#include <iostream>
#include <fstream>
#include <filesystem>
#include "MemoryModel.hpp"
#include "Measurements.h"



int main(int argc, char **argv) {
    // Check input arguments
    if (argc < 4) {
        std::cerr << "mem-pmod error: usage: mem-pmod FMEAS PORT1 PORT2 [-]" << std::endl;
        return -1;
    }
    // Parse input arguments
    std::string measurements_fname(argv[1]);
    if (!std::filesystem::exists(measurements_fname)) {
        std::cerr << "mem-pmod error: file " << std::filesystem::absolute(measurements_fname) << "does not exist."
                  << std::endl;
        return -1;
    }
    unsigned port1 = std::stoi(argv[2]);
    unsigned port2 = std::stoi(argv[3]);
    bool sweep_enabled = false;
    std::size_t sweep_n_samples = 0;
    for (int i = 4; i < argc; i++) {
        // Parse optional arguments
        std::string arg(argv[i]);
        if (arg.compare(0, 8, "--fsweep") == 0) {
            // Check if there are enough arguments
            // Additional arguments are NSAMPLES
            if (i + 1 >= argc) {
                std::cerr << "mem-pmod error: usage: mem-pmod [...] --sweep_enabled NSAMPLES";
                return -1;
            }
            sweep_n_samples = std::stoull(argv[i+1]);
            sweep_enabled = true;
            i += 1;
        }
    }
    // Initialize eigen with multithreading
    Eigen::initParallel();

    // Read measurements
    Measurements measurements = readMeasurements(measurements_fname, port1, port2);

    // Physical characteristics of memory
    const unsigned nsections = 23;
    const double total_length = 4074.0e-6;
    std::array<double, nsections> lengths{};
    lengths.fill(total_length / nsections);
    // Fit model
    MemoryModel<nsections> memory;
    std::cout << "Fitting " << std::filesystem::absolute(measurements_fname) << "..." << std::flush;
    memory = MemoryModel<nsections>::fit(measurements, lengths, pmod::optimization::CDESCENT);
    std::cout << " Done!\n"
              << "Memory parameters:\n"
              << memory << std::endl;
    // Print frequency sweep
    if (sweep_enabled) {
        std::ofstream fsweep("fsweep.txt");
        std::cout << "Saving frequency sweep on " << std::filesystem::absolute("fsweep.txt") << "...";
        memory.printLogarithmicFrequencySweep(
                fsweep,
                port1, port2,
                measurements.frequencies[0], measurements.frequencies[measurements.nsamples - 1],
                sweep_n_samples);
        fsweep.close();
        std::cout << " Done!" << std::endl;
    }
    return 0;
}
