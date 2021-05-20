#include "test.h"
#include "TSection.h"

static constexpr PULParameters BASIC_PARAMETERS = {1000.0, 50.0e-3, 0.0, 1000.0, 50.0e-3, 1.0e-12, 1.0e-6};

TEST(TestTSection, ParameterComputation) {
    constexpr double length = 1.0e-3;
    TSection section(length);
    PULParameters pul_parameters = BASIC_PARAMETERS;
    constexpr double frequency = 100.0e6;
    Parameters parameters = section.computeParameters(frequency, pul_parameters);
    Parameters expected = {
            length * pul_parameters.Rsac * sqrt(2 * M_PI * frequency),
            length * pul_parameters.Ls,
            1.0 / length * pul_parameters.Rpac * sqrt(2 * M_PI * frequency),
            1.0 / length * pul_parameters.Lp,
            length * pul_parameters.Cp};
    ASSERT_EQ(parameters, expected) << "Parameters mismatch";
}

TEST(TestTSection, ABCDMatrix) {
    double length = 1.0e-3;
    TSection section(length);
    PULParameters pul_parameters = BASIC_PARAMETERS;
    double frequency = 100.0e6;
    Matrix2 m = section.computeABCD(frequency, pul_parameters);
    // Approximate expected values
    ASSERT_COMPLEX_REL_NEAR(m(0, 0), 1.0, 0.01);
    ASSERT_COMPLEX_REL_NEAR(m(0, 1), 2.5, 0.01);
    ASSERT_COMPLEX_REL_NEAR(m(1, 0), 8.0e-7, 0.01);
    ASSERT_COMPLEX_REL_NEAR(m(1, 1), 1.0, 0.01);
}