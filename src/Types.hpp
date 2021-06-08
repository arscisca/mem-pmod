#ifndef MEM_PMOD_TYPES_HPP
#define MEM_PMOD_TYPES_HPP

#include <Eigen/Dense>
#include <complex>

using Complex = std::complex<double>;

template <std::size_t N>
using Matrix = Eigen::Matrix<Complex, N, N>;
using Matrix2 = Matrix<2>;

template <std::size_t N>
using Vector = Eigen::Vector<double, N>;

#endif //MEM_PMOD_TYPES_HPP
