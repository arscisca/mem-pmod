#ifndef MEM_PMOD_OPTIMIZATION_HPP
#define MEM_PMOD_OPTIMIZATION_HPP

#include <functional>
#include "Types.hpp"

namespace pmod::optimization {
    enum Algorithm {
        POWELL
    };

    template<std::size_t N>
    Vector<N> optimize(Algorithm algorithm, std::function<double(Vector<N>)> function, Vector<N> x0, double threshold);

        template<std::size_t N>
    Vector<N> powell(std::function<double(Vector<N>)> function, Vector<N> x0, double threshold) {
        // FIXME: actual implementation
        return x0;
    }

    template<std::size_t N>
    Vector<N> optimize(Algorithm algorithm, std::function<double(Vector<N>)> function, Vector<N> x0, double threshold) {
        switch(algorithm) {
            case POWELL:
                return powell<N>(function, x0, threshold);
            default:
                return Vector<N>::Zero();
        }
    }
}
#endif //MEM_PMOD_OPTIMIZATION_HPP
