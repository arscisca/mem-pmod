#ifndef MEM_PMOD_MEASUREMENTS_H
#define MEM_PMOD_MEASUREMENTS_H

#include <iostream>
#include <string>
#include <vector>
#include "Types.hpp"

struct Measurements {
    std::vector<double> frequencies;
    std::vector<Complex> Z11;
    std::vector<Complex> Z12;
    std::vector<Complex> Z21;
    std::vector<Complex> Z22;
};

// Read measurements
Measurements readMeasurements(std::istream &istream);
Measurements readMeasurements(const std::string &fname);

#endif //MEM_PMOD_MEASUREMENTS_H
