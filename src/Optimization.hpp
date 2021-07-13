#ifndef MEM_PMOD_OPTIMIZATION_HPP
#define MEM_PMOD_OPTIMIZATION_HPP

#include <functional>
#include "Types.hpp"

namespace pmod::optimization {
    const std::size_t MAX_ITERATIONS = 1e3;

    enum Algorithm {
        CDESCENT
    };

    template<std::size_t N>
    Vector<N> optimize(Algorithm algorithm, std::function<double(Vector<N>)> function, Vector<N> x0, double threshold);

    template<std::size_t N>
    Vector<N> cdescent(Vector<N> x0, std::function<double(Vector<N>)> function, double threshold) {
        std::size_t iteration = 0;
        Vector<N> x(x0);
        double y = function(x);
        std::size_t count_times_stable = 0;
        double last_y = y;
        bool converged = abs(y) <= threshold;
        Vector<N> x_min_absolute = x;
        double y_min_absolute = y;
        while (!converged && iteration < MAX_ITERATIONS) {
            // Descend along each direction
            std::size_t i = iteration % N;
            // Find minimum by sampling from x / 1000 to 1000 x
            std::size_t nsubsteps = 100;
            double factor_min = 1e-3;
            double factor_max = 1e+3;
            double step_factor = pow(factor_max / factor_min, 1.0 / nsubsteps);
            x[i] *= factor_min;
            Vector<N> x_min = x;
            double y_min = function(x);
            for (std::size_t j = 0; j < nsubsteps; j++) {
                y = function(x);
                if (y < y_min) {
                    x_min = x;
                    y_min = y;
                }
                x[i] *= step_factor;
            }
            x[i] = x_min[i];
            y = y_min;
            // Check for convergence
            if (y != 0 && (y - last_y) / y < 1e-6) {
                count_times_stable++;
            } else {
                count_times_stable = 0;
            }
            last_y = y;

            if (count_times_stable >= 2*N) {
                // Function has been stable in each coordinate, try to disturb it if not too close to the end
                if (iteration < MAX_ITERATIONS / 10) {
                    Vector<N> noise(Vector<N>::Random());
                    x = x.cwiseProduct(Vector<N>::Ones() + noise / 100);
                    // Add small quantity to wiggle away from zero
                    x += Vector<N>::Ones() * 1e-32;
                    y = function(x);
                }
            }
            if (y < y_min_absolute) {
                y_min_absolute = y;
                x_min_absolute = x;
            }
            converged = (abs(y) <= threshold) || count_times_stable >= 3*N;
            iteration++;
        }
        return x_min_absolute;
    }

    template<std::size_t N>
    Vector<N> optimize(Algorithm algorithm, std::function<double(Vector<N>)> function, Vector<N> x0, double threshold) {
        switch (algorithm) {
            case Algorithm::CDESCENT:
                return cdescent<N>(x0, function, threshold);
        }
        throw std::logic_error("Invalid algorithm");
    }
}
#endif //MEM_PMOD_OPTIMIZATION_HPP
