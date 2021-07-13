#ifndef MEM_PMOD_MEASUREMENTS_H
#define MEM_PMOD_MEASUREMENTS_H

#include <iostream>
#include <string>
#include <vector>
#include "Types.hpp"

struct Measurements {
    std::vector<double> frequencies;
    std::vector<Matrix2> Z;
    unsigned port1;
    unsigned port2;
    std::size_t nsamples;
};

// Read measurements
Measurements readMeasurements(std::istream &istream, unsigned port1, unsigned port2);
Measurements readMeasurements(const std::string &fname, unsigned port1, unsigned port2);

#endif //MEM_PMOD_MEASUREMENTS_H
