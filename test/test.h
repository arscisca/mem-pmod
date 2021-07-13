#ifndef MEM_PMOD_TEST_H
#define MEM_PMOD_TEST_H

#include <gtest/gtest.h>
#include <string>
const std::string test_data_path = "../data/test";

// Macros
#define ASSERT_REL_NEAR(val1, val2, relative_error) ASSERT_NEAR(val1, val2, val1 * relative_error)
//TODO: Test for phase!
#define ASSERT_COMPLEX_EQ(z1, z2) ASSERT_TRUE(z1 == z2)
#define ASSERT_COMPLEX_NEAR(z1, z2, mag_abs_error) ASSERT_NEAR(std::abs(z1), std::abs(z2), mag_abs_error)
#define ASSERT_COMPLEX_REL_NEAR(z1, z2, mag_rel_error) ASSERT_REL_NEAR(std::abs(z1), std::abs(z2), mag_rel_error)
#define ASSERT_VECTOR_NEAR(v1, v1_size, v2, v2_size, abs_error) ASSERT_EQ(v1_size, v2_size); \
for (std::size_t i = 0; i < v1_size; i++) ASSERT_NEAR(v1[i], v2[i], abs_error)
#endif //MEM_PMOD_TEST_H
