#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "MemoryModel.hpp"
#include "Measurements.h"

template <std::size_t NPowerPorts>
MemoryModel<NPowerPorts> fit(
        const std::string &fpath,
        const Measurements &measurements,
        const std::array<double, NPowerPorts> &lengths) {
    std::cout << "Fitting " << fpath << "..." << std::flush;
    auto tstart = std::chrono::high_resolution_clock::now();
    MemoryModel<NPowerPorts> memory(MemoryModel<NPowerPorts>::fit(measurements, lengths, pmod::optimization::CDESCENT));
    auto tend = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_s = tend - tstart;
    std::cout << " Done! (" << duration_s.count() << " s)\n"
              << "Memory parameters:\n"
              << memory << std::endl;
    return memory;
}

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
    bool export_enabled = false;
    std::string export_file = "";
    std::size_t sweep_n_samples = 0;
    for (int i = 4; i < argc; i++) {
        // Parse optional arguments
        std::string arg(argv[i]);
        if (arg.compare(0, 8, "--fsweep") == 0) {
            // Check if there are enough arguments
            // Additional arguments are NSAMPLES
            if (i + 1 >= argc) {
                std::cerr << "mem-pmod error: usage: mem-pmod [...] --fsweep NSAMPLES";
                return -1;
            }
            sweep_n_samples = std::stoull(argv[i+1]);
            sweep_enabled = true;
            i += 1;
        } else if (arg.compare(0, 2, "-e") == 0) {
            if (i + 1 >= argc) {
                std::cerr << "mem-pmod error: usage: mem-pmod [...] -e MODELNAME";
                return -1;
            }
            export_file = std::string(argv[i+1]);
            export_enabled = true;
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
    MemoryModel<nsections> memory(fit<nsections>(std::filesystem::absolute(measurements_fname), measurements, lengths));

    // Export model
    if (export_enabled) {
        std::cout << "Exporting model to "<< std::filesystem::absolute(export_file + ".model") << "...";
        memory.exportModel(export_file + ".model");
        std::cout << "Done!" << std::endl;
    }
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
