cmake_minimum_required(VERSION 3.17)
project(mem_pmod)

# Set compiler flags
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Werror -Wpedantic")
# Set target sources
set(SRC_DIR "src")
set(INC_DIR "src")
set(LIB_DIR "lib")

set(SOURCES
        ${INC_DIR}/Types.hpp
        ${INC_DIR}/Measurements.h ${SRC_DIR}/Measurements.cpp)
# Additional packages
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
# Main executable
add_executable(mem_pmod ${SRC_DIR}/main.cpp ${SOURCES})
target_include_directories(mem_pmod PRIVATE
        ${INC_DIR}
        ${LIB_DIR}/eigen
        )

# Libraries
target_link_libraries(mem_pmod Eigen3::Eigen)


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
        ${TEST_DIR}/test_Measurements.cpp)
# Create executable
add_executable(pmodtests ${SOURCES} ${TEST_SOURCES})
target_include_directories(pmodtests PRIVATE ${INC_DIR} ${LIB_DIR}/eigen)
target_link_libraries(pmodtests gtest_main Eigen3::Eigen)
include(GoogleTest)
gtest_discover_tests(pmodtests)