#include "Electrical.h"

std::string complexMatrixToCSV(const Matrix2 &m) {
    std::stringstream ss;
    ss << std::scientific << std::showpos;
    for (unsigned i = 0; i < m.rows(); i++) {
        for (unsigned j = 0; j < m.cols(); j++) {
            ss << m(i, j).real() << "," << m(i, j).imag() << ",";
        }
    }
    std::string result = ss.str();
    // Drop last comma
    result = result.substr(0, result.size() - 1);
    return result;
}

DataVector vectorize(const PULParameters &parameters) {
    return DataVector({{parameters.Rsdc, parameters.Rsdc, parameters.Ls,
            parameters.Rpdc, parameters.Rpac, parameters.Lp, parameters.Cp}});
}

PULParameters devectorize(const DataVector &data) {
    return {data[0], data[1], data[2], data[3], data[4], data[5], data[6]};
}

#ifdef PMOD_TEST
bool operator==(const Parameters &p1, const Parameters &p2) {
    return  p1.Rs == p2.Rs &&
            p1.Ls == p2.Ls &&
            p1.Rp == p2.Rp &&
            p1.Lp == p2.Lp &&
            p1.Cp == p2.Cp;
}

std::ostream &operator<<(std::ostream &ostream, const Parameters &parameters) {
    ostream << "{Rs=" << parameters.Rs << ", "
            << "Ls=" << parameters.Ls << ", "
            << "Rp=" << parameters.Rp << ", "
            << "Lp=" << parameters.Lp << ", "
            << "Cp=" << parameters.Cp << "}";
    return ostream;
}
#endif