#ifndef MEM_PMOD_TYPES_HPP
#define MEM_PMOD_TYPES_HPP

#include <Eigen/Dense>
#include <complex>
#include <Eigen/IterativeLinearSolvers>
using Complex = std::complex<double>;

template <std::size_t N>
using Matrix = Eigen::Matrix<Complex, N, N>;
using Matrix2 = Matrix<2>;

template <std::size_t N>
using Vector = Eigen::Matrix<double, N, 1>;

#endif //MEM_PMOD_TYPES_HPP
