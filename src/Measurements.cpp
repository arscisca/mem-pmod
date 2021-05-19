#include "Measurements.h"

#include <fstream>

Measurements readMeasurements(std::istream &istream) {
    Measurements measurements;
    double f;
    Complex Z11, Z12, Z21, Z22;
    while (istream >> f >> Z11 >> Z12 >> Z21 >> Z22) {
        measurements.frequencies.push_back(f);
        measurements.Z11.push_back(Z11);
        measurements.Z12.push_back(Z12);
        measurements.Z21.push_back(Z21);
        measurements.Z22.push_back(Z22);
    }
    return measurements;
}

Measurements readMeasurements(const std::string &fname) {
    std::fstream ifstream(fname, std::ios::in);
    Measurements m = readMeasurements(ifstream);
    ifstream.close();
    return m;
}
