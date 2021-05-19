#ifndef MEM_PMOD_TEST_H
#define MEM_PMOD_TEST_H

#include <string>
const std::string test_data_path = "../data/test";

// Macros
#define ASSERT_COMPLEX_EQ(z1, z2) ASSERT_TRUE(z1 == z2)
#endif //MEM_PMOD_TEST_H
