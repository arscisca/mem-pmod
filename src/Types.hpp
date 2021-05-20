#ifndef MEM_PMOD_TYPES_HPP
#define MEM_PMOD_TYPES_HPP

#include <Eigen/Dense>
#include <complex>
#include <iostream>

using namespace std::complex_literals;
using Complex = std::complex<double>;
using Matrix2 = Eigen::Matrix<std::complex<double>, 2, 2>;
using DataVector = Eigen::Vector<double, 7>;

#endif //MEM_PMOD_TYPES_HPP
