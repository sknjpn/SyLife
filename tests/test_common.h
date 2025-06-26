#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <string>

// Test environment for SyLife
class SyLifeTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // Initialize common test resources
        // Set up temporary directories for test data
        // Initialize logging for tests
    }
    
    void TearDown() override {
        // Clean up test resources
    }
};

// Base test fixture for SyLife tests
class SyLifeTestBase : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup for all tests
    }
    
    void TearDown() override {
        // Common cleanup for all tests
    }
    
    // Helper methods for test data generation
    std::string GetTestDataPath(const std::string& filename) {
        return "test_data/" + filename;
    }
    
    // Mock time for deterministic testing
    double GetMockTime() const {
        return mock_time_;
    }
    
    void AdvanceMockTime(double delta) {
        mock_time_ += delta;
    }
    
private:
    double mock_time_ = 0.0;
};

// Test data generators
namespace TestData {
    // Generate test JSON for assets
    std::string GenerateTestCellAssetJson();
    std::string GenerateTestPartAssetJson();
    std::string GenerateTestProteinAssetJson();
    
    // Generate test world data
    std::string GenerateTestWorldData();
}

// Custom matchers for SyLife types
MATCHER_P(Vec2Near, expected, "Vector2 is approximately equal") {
    const double tolerance = 1e-6;
    return std::abs(arg.x - expected.x) < tolerance && 
           std::abs(arg.y - expected.y) < tolerance;
}

MATCHER_P(ColorNear, expected, "Color is approximately equal") {
    const int tolerance = 1;
    return std::abs(arg.r - expected.r) <= tolerance &&
           std::abs(arg.g - expected.g) <= tolerance &&
           std::abs(arg.b - expected.b) <= tolerance &&
           std::abs(arg.a - expected.a) <= tolerance;
}