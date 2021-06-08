#include "test.h"
#include "Measurements.h"
using namespace std::complex_literals;
TEST(Measurements, Input) {
    // Read measurements from text file
    Measurements m = readMeasurements(test_data_path + "/fakemeasurements.txt", 2, 22);
    ASSERT_EQ(m.frequencies.size(), 50ull) << "Incorrect size of sampled data";
    for (size_t i = 0; i < m.frequencies.size(); i++) {
        ASSERT_DOUBLE_EQ(m.frequencies[i], (i + 1) * 100.0e6) << "Frequency does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z[i](0, 0), Complex(i + 1) * (1.0 + 1.0i)) << "Z11 does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z[i](0, 1), Complex(i + 1) * (1.0 + 1.0i)) << "Z12 does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z[i](1, 0), Complex(i + 1) * (1.0 + 1.0i)) << "Z21 does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z[i](1, 1), Complex(i + 1) * (1.0 + 1.0i)) << "Z22 does not have the expected value";
    }
}
