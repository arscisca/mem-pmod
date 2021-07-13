#include "test.h"
#include "Optimization.hpp"

double paraboloid(Vector<2> x) {
    return x[0] * x[0] + x[1] * x[1];
}

TEST(TestOptimization, CDESCENT_Paraboloid) {
    Vector<2> x0 = {10, 10};
    double threshold = 1e-32;
    Vector<2> x_optim = pmod::optimization::optimize<2>(pmod::optimization::Algorithm::CDESCENT, paraboloid, x0,
                                                        threshold);
    // Function has to converge according to threshold ONLY because the function has a single, global minimum.
    //
    // Expected optimized coordinate is zero but coordinates will converge by sqrt(threshold) because the function is
    // quadratic.
    ASSERT_NEAR(paraboloid(x_optim), 0, threshold);
    ASSERT_VECTOR_NEAR(x_optim, 2, Vector<2>::Zero(), 2, sqrt(threshold));
}
