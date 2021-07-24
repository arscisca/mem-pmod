cmake_minimum_required(VERSION 3.17)
project(mem_pmod)

# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Werror -Wall -Wpedantic -Wextra")
# Set target sources
set(SRC_DIR "src")
set(INC_DIR "src")
set(LIB_DIR "lib")

set(SOURCES
        ${SRC_DIR}/main.cpp
        ${INC_DIR}/Types.hpp
        ${INC_DIR}/Measurements.h ${SRC_DIR}/Measurements.cpp
        ${INC_DIR}/Electrical.h ${SRC_DIR}/Electrical.cpp
        ${INC_DIR}/MemoryModel.hpp
        ${INC_DIR}/TSection.h ${SRC_DIR}/TSection.cpp
        ${INC_DIR}/Optimization.hpp
        )
# Additional packages
find_package(Eigen3 3.3 REQUIRED)
# Main executable
add_executable(mem_pmod ${SOURCES})
target_include_directories(mem_pmod PRIVATE
        ${INC_DIR}
        )

# Libraries
target_link_libraries(mem_pmod PRIVATE Eigen3::Eigen pthread)


# -- GTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
# Set sources
set(TEST_DIR "test")
set(TEST_SOURCES
        ${TEST_DIR}/test.h
        ${TEST_DIR}/test_Measurements.cpp
        ${TEST_DIR}/test_TSection.cpp
        ${TEST_DIR}/test_Optimization.cpp
        )
# Create executable
add_executable(pmodtests ${SOURCES} ${TEST_SOURCES})
target_compile_definitions(pmodtests PUBLIC PMOD_TEST)
target_include_directories(pmodtests PRIVATE ${INC_DIR} ${EIGEN3_INCLUDE_DIR})
target_link_libraries(pmodtests gtest_main Eigen3::Eigen)
include(GoogleTest)
gtest_discover_tests(pmodtests)