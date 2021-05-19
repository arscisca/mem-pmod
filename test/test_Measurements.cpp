#include <gtest/gtest.h>
#include "test.h"
#include "Measurements.h"

TEST(Measurements, Input) {
    // Read measurements from text file
    Measurements m = readMeasurements(test_data_path + "/measurements.txt");
    ASSERT_EQ(m.frequencies.size(), 50ull) << "Incorrect size of sampled data";
    for (size_t i = 0; i < m.frequencies.size(); i++) {
        ASSERT_DOUBLE_EQ(m.frequencies[i], (i + 1) * 100.0e6) << "Frequency does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z11[i], Complex(i + 1) * (1.0 + 1.0i)) << "Z11 does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z12[i], Complex(i + 1) * (1.0 + 1.0i)) << "Z12 does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z21[i], Complex(i + 1) * (1.0 + 1.0i)) << "Z21 does not have the expected value";
        ASSERT_COMPLEX_EQ(m.Z22[i], Complex(i + 1) * (1.0 + 1.0i)) << "Z22 does not have the expected value";
    }
}
