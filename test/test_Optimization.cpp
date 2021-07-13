#include "test.h"
#include "Optimization.hpp"

double paraboloid(Vector<2> x) {
    return x[0] * x[0] + x[1] * x[1];
}

TEST(TestOptimization, CDESCENT_Paraboloid) {
    Vector<2> x0 = {10, 10};
    Vector<2> x_optim = pmod::optimization::optimize<2>(pmod::optimization::Algorithm::CDESCENT, paraboloid, x0, 1e-32);
    ASSERT_VECTOR_NEAR(x_optim, 2, Vector<2>::Zero(), 2, 1e-16);
}
