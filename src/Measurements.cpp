#include "Measurements.h"

#include <fstream>

Measurements readMeasurements(std::istream &istream) {
    Measurements measurements;
    double f;
    Complex Z11, Z12, Z21, Z22;
    while (istream >> f >> Z11 >> Z12 >> Z21 >> Z22) {
        measurements.frequencies.push_back(f);
        measurements.Z.emplace_back(Matrix2{{Z11, Z12}, {Z21, Z22}});
    }
    return measurements;
}

Measurements readMeasurements(const std::string &fname) {
    std::fstream ifstream(fname, std::ios::in);
    Measurements m = readMeasurements(ifstream);
    ifstream.close();
    return m;
}
