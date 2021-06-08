#include "Measurements.h"

#include <fstream>

Measurements readMeasurements(std::istream &istream, unsigned port1, unsigned port2) {
    Measurements measurements;
    double f;
    Complex Z11, Z12, Z21, Z22;
    // Ignore header line
    istream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (istream >> f >> Z11 >> Z12 >> Z21 >> Z22) {
        measurements.frequencies.push_back(f);
        measurements.Z.emplace_back(Matrix2{{Z11, Z12}, {Z21, Z22}});
    }
    measurements.port1 = port1;
    measurements.port2 = port2;
    return measurements;
}

Measurements readMeasurements(const std::string &fname, unsigned port1, unsigned port2) {
    std::fstream ifstream(fname, std::ios::in);
    Measurements m = readMeasurements(ifstream, port1, port2);
    ifstream.close();
    return m;
}
