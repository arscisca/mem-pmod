#include "test.h"
#include "MemoryModel.hpp"

TEST(MemoryModel, ImportExport) {
    // Generate a memory model
    constexpr std::size_t nsections = 32;
    std::array<double, nsections> lengths;
    lengths.fill(1e-6);
    PULParameters pul_parameters = reference_pul_parameters;
    MemoryModel<nsections> model;
    model.setSectionLengths(lengths);
    model.setPULParameters(pul_parameters, 10e9);
    // Export model to temporary file
    model.exportModel("test.model");
    // Import model
    MemoryModel<nsections> imported = MemoryModel<nsections>::importModel("test.model");
    // Compare data
    ASSERT_EQ(model.getPULParameters(), imported.getPULParameters());
    // Delete file
    std::remove("test.model");
}