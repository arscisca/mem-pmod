#ifndef MEM_PMOD_OPTIMIZATION_HPP
#define MEM_PMOD_OPTIMIZATION_HPP

#include <functional>
#include "Types.hpp"

namespace pmod::optimization {
    const std::size_t MAX_ITERATIONS = 1024;

    enum Algorithm {
        CDESCENT
    };

    template<std::size_t N>
    Vector<N> optimize(Algorithm algorithm, std::function<double(Vector<N>)> function, Vector<N> x0, double threshold);

    template<std::size_t N>
    Vector<N> cdescent(Vector<N> x0, std::function<double(Vector<N>)> function, double threshold) {
        constexpr double step_factor = 0.01;
        std::size_t iteration = 0;
        Vector<N> x(x0);
        double y = function(x);
        while (abs(y) > threshold && iteration < MAX_ITERATIONS) {
            // Descend along each direction
            for (std::size_t i = 0; i < N; i++) {
                // Initialize step
                Vector<N> step = Vector<N>::Zero();
                step[i] = abs(x[i]) * step_factor;
                // Check whether it's best to go left or right
                Vector<N> new_x = x + step;
                double new_y = function(new_x);
                if (new_y < y) {
                    // Go left
                    step = -1 * step;
                    new_x = x + step;
                }
                // Advance
                do {
                    x = new_x;
                    y = new_y;
                    new_x = x + step;
                    new_y = function(new_x);
                    iteration++;
                } while (new_y < y && iteration < MAX_ITERATIONS / (6 * N));
            }
        }
        return x;
    }

    template<std::size_t N>
    Vector<N> optimize(Algorithm algorithm, std::function<double(Vector<N>)> function, Vector<N> x0, double threshold) {
        return cdescent<N>(x0, function, threshold);
    }
}
#endif //MEM_PMOD_OPTIMIZATION_HPP
