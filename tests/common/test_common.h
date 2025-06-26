#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sylife/core/common.h"
#include "sylife/core/world.h"
#include "test_fixtures.h"
#include "test_generators.h"
#include "test_matchers.h"
#include "test_mocks.h"
#include <memory>
#include <chrono>
#include <random>

namespace sylife::testing {

/**
 * @brief Base test environment for SyLife tests
 */
class SyLifeTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override;
    void TearDown() override;
    
    static void InitializeGlobalResources();
    static void CleanupGlobalResources();
};

/**
 * @brief Base test fixture for all SyLife tests
 */
class SyLifeTestBase : public ::testing::Test {
protected:
    void SetUp() override;
    void TearDown() override;
    
    /**
     * @brief Get test data directory path
     * @param filename Filename within test data directory
     * @return Full path to test data file
     */
    std::string getTestDataPath(const std::string& filename = "") const;
    
    /**
     * @brief Get temporary directory for test files
     * @return Path to temporary directory
     */
    std::string getTempPath() const;
    
    /**
     * @brief Create temporary file with content
     * @param content File content
     * @param extension File extension
     * @return Path to created file
     */
    std::string createTempFile(const std::string& content, const std::string& extension = ".tmp");
    
    /**
     * @brief Clean up temporary files
     */
    void cleanupTempFiles();
    
    /**
     * @brief Mock time for deterministic testing
     * @return Current mock time
     */
    double getMockTime() const;
    
    /**
     * @brief Advance mock time
     * @param delta Time to advance
     */
    void advanceMockTime(double delta);
    
    /**
     * @brief Reset mock time to zero
     */
    void resetMockTime();
    
    /**
     * @brief Get random number generator with fixed seed
     * @return Reference to random number generator
     */
    std::mt19937& getRandom();
    
    /**
     * @brief Seed random number generator
     * @param seed Seed value
     */
    void seedRandom(unsigned int seed);

private:
    mutable double mockTime_;
    std::mt19937 randomGenerator_;
    std::vector<std::string> tempFiles_;
    std::string tempDir_;
    
    void initializeTempDirectory();
};

/**
 * @brief Test fixture with isolated world instance
 */
class WorldTestBase : public SyLifeTestBase {
protected:
    void SetUp() override;
    void TearDown() override;
    
    /**
     * @brief Get test world instance
     * @return Pointer to world instance
     */
    core::World* getWorld();
    
    /**
     * @brief Initialize world with test configuration
     * @param width World width in tiles
     * @param height World height in tiles
     */
    void initializeWorld(int width = 50, int height = 50);
    
    /**
     * @brief Run simulation for specified number of steps
     * @param steps Number of simulation steps
     * @param deltaTime Time step (default: constants::DELTA_TIME)
     */
    void runSimulation(int steps, double deltaTime = constants::DELTA_TIME);

private:
    std::unique_ptr<core::World> world_;
    bool worldInitialized_;
};

/**
 * @brief Performance test base with timing utilities
 */
class PerformanceTestBase : public SyLifeTestBase {
protected:
    void SetUp() override;
    void TearDown() override;
    
    /**
     * @brief Start performance timing
     * @param label Timer label
     */
    void startTimer(const std::string& label = "default");
    
    /**
     * @brief Stop performance timing and get duration
     * @param label Timer label
     * @return Duration in microseconds
     */
    std::chrono::microseconds stopTimer(const std::string& label = "default");
    
    /**
     * @brief Get average time for multiple runs
     * @param func Function to benchmark
     * @param iterations Number of iterations
     * @return Average duration in microseconds
     */
    template<typename Func>
    std::chrono::microseconds benchmark(Func&& func, int iterations = 1000) {
        auto start = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < iterations; ++i) {
            func();
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto total = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        return total / iterations;
    }
    
    /**
     * @brief Assert performance is within expected bounds
     * @param actualDuration Actual measured duration
     * @param expectedMaxDuration Maximum expected duration
     * @param label Performance test label
     */
    void assertPerformance(std::chrono::microseconds actualDuration,
                          std::chrono::microseconds expectedMaxDuration,
                          const std::string& label = "");

private:
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> timers_;
    std::vector<std::pair<std::string, std::chrono::microseconds>> performanceResults_;
};

/**
 * @brief Network test base with mock networking
 */
class NetworkTestBase : public SyLifeTestBase {
protected:
    void SetUp() override;
    void TearDown() override;
    
    /**
     * @brief Create mock WebSocket server
     * @param port Server port
     * @return Mock server instance
     */
    std::unique_ptr<MockWebSocketServer> createMockServer(int port = 8080);
    
    /**
     * @brief Create mock WebSocket client
     * @return Mock client instance
     */
    std::unique_ptr<MockWebSocketClient> createMockClient();
    
    /**
     * @brief Simulate network delay
     * @param milliseconds Delay in milliseconds
     */
    void simulateNetworkDelay(int milliseconds);

private:
    std::vector<std::unique_ptr<MockWebSocketServer>> mockServers_;
    std::vector<std::unique_ptr<MockWebSocketClient>> mockClients_;
};

/**
 * @brief Security test base with authentication and authorization testing
 */
class SecurityTestBase : public SyLifeTestBase {
protected:
    void SetUp() override;
    void TearDown() override;
    
    /**
     * @brief Create test security manager
     * @return Security manager instance
     */
    std::unique_ptr<network::SecurityManager> createSecurityManager();
    
    /**
     * @brief Generate test authentication token
     * @param clientId Client identifier
     * @param level Security level
     * @return Authentication token
     */
    std::string generateTestToken(const std::string& clientId = "test",
                                 network::SecurityLevel level = network::SecurityLevel::PUBLIC);
    
    /**
     * @brief Create malicious input for testing
     * @param type Type of malicious input
     * @return Malicious test string
     */
    std::string createMaliciousInput(const std::string& type);

private:
    std::unique_ptr<network::SecurityManager> securityManager_;
};

// Utility macros for common test assertions
#define ASSERT_VALID_OBJECT(obj) \
    ASSERT_TRUE((obj) != nullptr) << "Object should not be null"; \
    ASSERT_TRUE((obj)->isValid()) << "Object should be valid"

#define EXPECT_VALID_OBJECT(obj) \
    EXPECT_TRUE((obj) != nullptr) << "Object should not be null"; \
    EXPECT_TRUE((obj)->isValid()) << "Object should be valid"

#define ASSERT_NEAR_VEC2(actual, expected, tolerance) \
    ASSERT_NEAR((actual).x, (expected).x, (tolerance)) << "X component mismatch"; \
    ASSERT_NEAR((actual).y, (expected).y, (tolerance)) << "Y component mismatch"

#define EXPECT_NEAR_VEC2(actual, expected, tolerance) \
    EXPECT_NEAR((actual).x, (expected).x, (tolerance)) << "X component mismatch"; \
    EXPECT_NEAR((actual).y, (expected).y, (tolerance)) << "Y component mismatch"

#define ASSERT_COLOR_NEAR(actual, expected, tolerance) \
    ASSERT_NEAR((actual).r, (expected).r, (tolerance)) << "Red component mismatch"; \
    ASSERT_NEAR((actual).g, (expected).g, (tolerance)) << "Green component mismatch"; \
    ASSERT_NEAR((actual).b, (expected).b, (tolerance)) << "Blue component mismatch"; \
    ASSERT_NEAR((actual).a, (expected).a, (tolerance)) << "Alpha component mismatch"

#define EXPECT_COLOR_NEAR(actual, expected, tolerance) \
    EXPECT_NEAR((actual).r, (expected).r, (tolerance)) << "Red component mismatch"; \
    EXPECT_NEAR((actual).g, (expected).g, (tolerance)) << "Green component mismatch"; \
    EXPECT_NEAR((actual).b, (expected).b, (tolerance)) << "Blue component mismatch"; \
    EXPECT_NEAR((actual).a, (expected).a, (tolerance)) << "Alpha component mismatch"

#define ASSERT_JSON_HAS_FIELD(json, field) \
    ASSERT_TRUE((json).contains(field)) << "JSON should contain field: " << field

#define EXPECT_JSON_HAS_FIELD(json, field) \
    EXPECT_TRUE((json).contains(field)) << "JSON should contain field: " << field

#define ASSERT_PERFORMANCE_WITHIN(actual, expected, label) \
    ASSERT_LE((actual).count(), (expected).count()) \
        << "Performance test '" << (label) << "' exceeded expected time: " \
        << (actual).count() << "us > " << (expected).count() << "us"

#define EXPECT_PERFORMANCE_WITHIN(actual, expected, label) \
    EXPECT_LE((actual).count(), (expected).count()) \
        << "Performance test '" << (label) << "' exceeded expected time: " \
        << (actual).count() << "us > " << (expected).count() << "us"

// Test data generation helpers
namespace TestData {
    /**
     * @brief Generate test JSON for cell asset
     * @return JSON string
     */
    std::string generateCellAssetJson();
    
    /**
     * @brief Generate test JSON for part asset
     * @return JSON string
     */
    std::string generatePartAssetJson();
    
    /**
     * @brief Generate test JSON for protein asset
     * @return JSON string
     */
    std::string generateProteinAssetJson();
    
    /**
     * @brief Generate test world configuration
     * @return JSON string
     */
    std::string generateWorldConfigJson();
    
    /**
     * @brief Generate random valid asset name
     * @return Asset name
     */
    std::string generateRandomAssetName();
    
    /**
     * @brief Generate random Vec2 within bounds
     * @param minX Minimum X value
     * @param maxX Maximum X value
     * @param minY Minimum Y value
     * @param maxY Maximum Y value
     * @return Random Vec2
     */
    s3d::Vec2 generateRandomVec2(double minX = 0.0, double maxX = 100.0,
                                 double minY = 0.0, double maxY = 100.0);
    
    /**
     * @brief Generate random color
     * @return Random color
     */
    s3d::ColorF generateRandomColor();
}

} // namespace sylife::testing

// Global test environment setup
::testing::Environment* const g_sylife_env = 
    ::testing::AddGlobalTestEnvironment(new sylife::testing::SyLifeTestEnvironment);