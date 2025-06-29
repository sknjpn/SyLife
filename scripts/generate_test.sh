#!/bin/bash

# Test Generator
# Automatically generates comprehensive test files for existing source files

set -e

SOURCE_FILE=""
TEST_TYPE="unit"
MOCK_DEPENDENCIES=false

usage() {
    echo "Usage: $0 -s <source_file> [-t <test_type>] [-m]"
    echo ""
    echo "Options:"
    echo "  -s <file>      Source file to generate tests for"
    echo "  -t <type>      Test type: unit, integration, performance (default: unit)"
    echo "  -m             Generate mock dependencies"
    echo ""
    echo "Examples:"
    echo "  $0 -s src/World.cpp"
    echo "  $0 -s modules/logger/src/file_sink.cpp -t unit -m"
    exit 1
}

# Parse command line arguments
while getopts "s:t:mh" opt; do
    case $opt in
        s) SOURCE_FILE="$OPTARG" ;;
        t) TEST_TYPE="$OPTARG" ;;
        m) MOCK_DEPENDENCIES=true ;;
        h) usage ;;
        *) usage ;;
    esac
done

# Validate required arguments
if [[ -z "$SOURCE_FILE" ]]; then
    echo "❌ Error: Source file is required"
    usage
fi

if [[ ! -f "$SOURCE_FILE" ]]; then
    echo "❌ Error: Source file '$SOURCE_FILE' does not exist"
    exit 1
fi

# Extract file information
SOURCE_DIR=$(dirname "$SOURCE_FILE")
SOURCE_NAME=$(basename "$SOURCE_FILE" .cpp)
SOURCE_EXT="${SOURCE_FILE##*.}"

# Determine test location based on source structure
if [[ "$SOURCE_FILE" =~ ^modules/([^/]+)/src/(.+)\.cpp$ ]]; then
    MODULE_NAME="${BASH_REMATCH[1]}"
    FILE_NAME="${BASH_REMATCH[2]}"
    TEST_DIR="modules/$MODULE_NAME/tests"
    HEADER_PATH="modules/$MODULE_NAME/include/sylife/$MODULE_NAME/${FILE_NAME}.h"
    INCLUDE_PATH="sylife/$MODULE_NAME/${FILE_NAME}.h"
elif [[ "$SOURCE_FILE" =~ ^src/(.+)\.cpp$ ]]; then
    FILE_NAME="${BASH_REMATCH[1]}"
    TEST_DIR="tests"
    HEADER_PATH="src/${FILE_NAME}.h"
    INCLUDE_PATH="${FILE_NAME}.h"
else
    echo "❌ Error: Unsupported source file structure"
    exit 1
fi

# Create test directory
mkdir -p "$TEST_DIR"

# Test file paths
TEST_FILE="$TEST_DIR/test_${FILE_NAME,,}.cpp"
MOCK_FILE="$TEST_DIR/mock_${FILE_NAME,,}.h"

echo "🔧 Generating tests for: $SOURCE_FILE"
echo "📁 Test directory: $TEST_DIR"
echo "📄 Test file: $TEST_FILE"
if [[ "$MOCK_DEPENDENCIES" == true ]]; then
    echo "🎭 Mock file: $MOCK_FILE"
fi

# Analyze source file for class/function extraction
CLASS_NAME=""
FUNCTIONS=()
INCLUDES=()

if [[ -f "$HEADER_PATH" ]]; then
    # Extract class name
    CLASS_NAME=$(grep -E "^class\s+\w+" "$HEADER_PATH" | head -1 | sed 's/.*class\s\+\([A-Za-z_][A-Za-z0-9_]*\).*/\1/')
    
    # Extract public methods
    mapfile -t FUNCTIONS < <(grep -E "^\s*(virtual\s+)?\w+.*\(" "$HEADER_PATH" | grep -v "^//" | sed 's/^\s*//' | head -10)
    
    # Extract includes
    mapfile -t INCLUDES < <(grep -E "^#include" "$HEADER_PATH" | head -5)
fi

# Generate test file based on type
case "$TEST_TYPE" in
    "unit")
        generate_unit_test
        ;;
    "integration")
        generate_integration_test
        ;;
    "performance")
        generate_performance_test
        ;;
    *)
        echo "❌ Error: Unknown test type '$TEST_TYPE'"
        exit 1
        ;;
esac

function generate_unit_test() {
cat > "$TEST_FILE" << EOF
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "$INCLUDE_PATH"

$(if [[ "$MOCK_DEPENDENCIES" == true ]]; then echo "#include \"mock_${FILE_NAME,,}.h\""; fi)

using namespace testing;

$(if [[ -n "$CLASS_NAME" ]]; then
cat << CLASSTEST
class ${CLASS_NAME}Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test fixtures
        subject = std::make_unique<$CLASS_NAME>();
    }
    
    void TearDown() override {
        // Cleanup test fixtures
        subject.reset();
    }
    
    std::unique_ptr<$CLASS_NAME> subject;
};

// Construction and Destruction Tests
TEST_F(${CLASS_NAME}Test, DefaultConstruction) {
    EXPECT_NE(subject, nullptr);
}

TEST_F(${CLASS_NAME}Test, CopyConstruction) {
    if constexpr (std::is_copy_constructible_v<$CLASS_NAME>) {
        $CLASS_NAME original;
        $CLASS_NAME copy(original);
        
        // Verify copy is independent
        EXPECT_NE(&original, &copy);
    }
}

TEST_F(${CLASS_NAME}Test, MoveConstruction) {
    if constexpr (std::is_move_constructible_v<$CLASS_NAME>) {
        $CLASS_NAME original;
        $CLASS_NAME moved(std::move(original));
        
        // Verify move was successful
        // Add specific checks based on class behavior
    }
}

TEST_F(${CLASS_NAME}Test, Assignment) {
    if constexpr (std::is_copy_assignable_v<$CLASS_NAME>) {
        $CLASS_NAME original;
        $CLASS_NAME assigned;
        
        assigned = original;
        
        // Verify assignment
        EXPECT_NE(&original, &assigned);
    }
}

// Method Tests
$(for func in "${FUNCTIONS[@]}"; do
    func_name=$(echo "$func" | sed 's/.*\s\+\([A-Za-z_][A-Za-z0-9_]*\)\s*(.*/\1/')
    if [[ -n "$func_name" && "$func_name" != "$CLASS_NAME" ]]; then
        echo "TEST_F(${CLASS_NAME}Test, ${func_name}BasicFunctionality) {"
        echo "    // TODO: Implement test for $func_name"
        echo "    // EXPECT_..."
        echo "}"
        echo ""
    fi
done)

// Edge Cases and Error Handling
TEST_F(${CLASS_NAME}Test, HandlesInvalidInput) {
    // TODO: Test with invalid/boundary inputs
    // EXPECT_THROW(subject->method(invalid_input), std::exception);
}

TEST_F(${CLASS_NAME}Test, HandlesEmptyState) {
    // TODO: Test behavior with empty/uninitialized state
}

// State Verification Tests
TEST_F(${CLASS_NAME}Test, StateConsistency) {
    // TODO: Verify object state remains consistent after operations
}

// Performance Tests
TEST_F(${CLASS_NAME}Test, PerformanceBasicOperations) {
    constexpr size_t iterations = 10000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        // TODO: Add performance-critical operation
        // subject->criticalMethod();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should complete within reasonable time
    EXPECT_LT(duration.count(), 100000); // Less than 100ms
}
CLASSTEST
else
cat << FUNCTEST
// Function Tests for $FILE_NAME

class ${FILE_NAME^}Test : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test environment
    }
    
    void TearDown() override {
        // Cleanup test environment
    }
};

$(for func in "${FUNCTIONS[@]}"; do
    func_name=$(echo "$func" | sed 's/.*\s\+\([A-Za-z_][A-Za-z0-9_]*\)\s*(.*/\1/')
    if [[ -n "$func_name" ]]; then
        echo "TEST_F(${FILE_NAME^}Test, ${func_name}BasicFunctionality) {"
        echo "    // TODO: Implement test for $func_name"
        echo "    // EXPECT_..."
        echo "}"
        echo ""
    fi
done)

// Edge Cases
TEST_F(${FILE_NAME^}Test, HandlesEdgeCases) {
    // TODO: Test edge cases and boundary conditions
}

// Error Handling
TEST_F(${FILE_NAME^}Test, ErrorHandling) {
    // TODO: Test error conditions
    // EXPECT_THROW(..., std::exception);
}
FUNCTEST
fi)

// Integration Points
TEST_F($(if [[ -n "$CLASS_NAME" ]]; then echo "${CLASS_NAME}Test"; else echo "${FILE_NAME^}Test"; fi), IntegrationWithDependencies) {
    // TODO: Test integration with other components
}

// Thread Safety (if applicable)
TEST_F($(if [[ -n "$CLASS_NAME" ]]; then echo "${CLASS_NAME}Test"; else echo "${FILE_NAME^}Test"; fi), ThreadSafety) {
    // TODO: Test thread safety if class is meant to be thread-safe
    /*
    std::vector<std::thread> threads;
    std::atomic<int> counter{0};
    
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back([&]() {
            for (int j = 0; j < 1000; ++j) {
                // Thread-safe operation
                counter++;
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_EQ(counter.load(), 10000);
    */
}

// Memory Management
TEST_F($(if [[ -n "$CLASS_NAME" ]]; then echo "${CLASS_NAME}Test"; else echo "${FILE_NAME^}Test"; fi), MemoryManagement) {
    // TODO: Test for memory leaks using valgrind or AddressSanitizer
    // This test should be run with memory debugging tools
}
EOF
}

function generate_integration_test() {
cat > "$TEST_FILE" << EOF
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "$INCLUDE_PATH"

// Include related components for integration testing
// TODO: Add necessary includes for integration

using namespace testing;

class ${CLASS_NAME:-${FILE_NAME^}}IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup integration test environment
        // Initialize real dependencies
    }
    
    void TearDown() override {
        // Cleanup integration test environment
    }
};

// Integration with Real Dependencies
TEST_F(${CLASS_NAME:-${FILE_NAME^}}IntegrationTest, WorksWithRealDependencies) {
    // TODO: Test with real implementations of dependencies
}

// End-to-End Scenarios
TEST_F(${CLASS_NAME:-${FILE_NAME^}}IntegrationTest, EndToEndScenario) {
    // TODO: Test complete workflow from start to finish
}

// Data Flow Integration
TEST_F(${CLASS_NAME:-${FILE_NAME^}}IntegrationTest, DataFlowIntegration) {
    // TODO: Test data flow through multiple components
}

// Error Propagation
TEST_F(${CLASS_NAME:-${FILE_NAME^}}IntegrationTest, ErrorPropagation) {
    // TODO: Test how errors propagate through the system
}

// Configuration Integration
TEST_F(${CLASS_NAME:-${FILE_NAME^}}IntegrationTest, ConfigurationIntegration) {
    // TODO: Test with different configurations
}
EOF
}

function generate_performance_test() {
cat > "$TEST_FILE" << EOF
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include "$INCLUDE_PATH"
#include <vector>
#include <random>
#include <chrono>

// Performance Tests for $FILE_NAME

class ${CLASS_NAME:-${FILE_NAME^}}PerformanceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup performance test environment
        $(if [[ -n "$CLASS_NAME" ]]; then echo "subject = std::make_unique<$CLASS_NAME>();"; fi)
    }
    
    void TearDown() override {
        // Cleanup performance test environment
        $(if [[ -n "$CLASS_NAME" ]]; then echo "subject.reset();"; fi)
    }
    
    $(if [[ -n "$CLASS_NAME" ]]; then echo "std::unique_ptr<$CLASS_NAME> subject;"; fi)
};

// Benchmark Functions
$(if [[ -n "$CLASS_NAME" ]]; then
cat << PERFTEST
static void BM_${CLASS_NAME}_Construction(benchmark::State& state) {
    for (auto _ : state) {
        $CLASS_NAME obj;
        benchmark::DoNotOptimize(obj);
    }
}
BENCHMARK(BM_${CLASS_NAME}_Construction);

static void BM_${CLASS_NAME}_CopyConstruction(benchmark::State& state) {
    $CLASS_NAME original;
    for (auto _ : state) {
        $CLASS_NAME copy(original);
        benchmark::DoNotOptimize(copy);
    }
}
BENCHMARK(BM_${CLASS_NAME}_CopyConstruction);

$(for func in "${FUNCTIONS[@]}"; do
    func_name=$(echo "$func" | sed 's/.*\s\+\([A-Za-z_][A-Za-z0-9_]*\)\s*(.*/\1/')
    if [[ -n "$func_name" && "$func_name" != "$CLASS_NAME" ]]; then
        echo "static void BM_${CLASS_NAME}_${func_name}(benchmark::State& state) {"
        echo "    $CLASS_NAME obj;"
        echo "    for (auto _ : state) {"
        echo "        // TODO: Benchmark $func_name method"
        echo "        // benchmark::DoNotOptimize(obj.$func_name());"
        echo "    }"
        echo "}"
        echo "BENCHMARK(BM_${CLASS_NAME}_${func_name});"
        echo ""
    fi
done)
PERFTEST
fi)

// Memory Usage Tests
TEST_F(${CLASS_NAME:-${FILE_NAME^}}PerformanceTest, MemoryUsage) {
    constexpr size_t object_count = 10000;
    
    auto start_memory = getCurrentMemoryUsage();
    
    std::vector<$(if [[ -n "$CLASS_NAME" ]]; then echo "$CLASS_NAME"; else echo "int"; fi)> objects;
    objects.reserve(object_count);
    
    for (size_t i = 0; i < object_count; ++i) {
        objects.emplace_back($(if [[ -n "$CLASS_NAME" ]]; then echo ""; else echo "i"; fi));
    }
    
    auto end_memory = getCurrentMemoryUsage();
    auto memory_per_object = (end_memory - start_memory) / object_count;
    
    // TODO: Verify memory usage is within expected bounds
    // EXPECT_LT(memory_per_object, expected_size);
    
    std::cout << "Memory per object: " << memory_per_object << " bytes" << std::endl;
}

// Throughput Tests
TEST_F(${CLASS_NAME:-${FILE_NAME^}}PerformanceTest, Throughput) {
    constexpr size_t iterations = 1000000;
    constexpr auto test_duration = std::chrono::seconds(1);
    
    size_t operations_completed = 0;
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + test_duration;
    
    while (std::chrono::high_resolution_clock::now() < end) {
        // TODO: Perform operation to measure throughput
        // subject->operation();
        operations_completed++;
    }
    
    auto actual_duration = std::chrono::high_resolution_clock::now() - start;
    auto ops_per_second = operations_completed * 1000.0 / 
                         std::chrono::duration_cast<std::chrono::milliseconds>(actual_duration).count();
    
    std::cout << "Operations per second: " << ops_per_second << std::endl;
    
    // TODO: Verify throughput meets requirements
    // EXPECT_GT(ops_per_second, minimum_required_ops);
}

// Latency Tests
TEST_F(${CLASS_NAME:-${FILE_NAME^}}PerformanceTest, Latency) {
    constexpr size_t iterations = 10000;
    std::vector<std::chrono::nanoseconds> latencies;
    latencies.reserve(iterations);
    
    for (size_t i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        
        // TODO: Perform operation to measure latency
        // subject->operation();
        
        auto end = std::chrono::high_resolution_clock::now();
        latencies.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start));
    }
    
    // Calculate statistics
    std::sort(latencies.begin(), latencies.end());
    auto p50 = latencies[iterations * 50 / 100];
    auto p95 = latencies[iterations * 95 / 100];
    auto p99 = latencies[iterations * 99 / 100];
    
    std::cout << "Latency P50: " << p50.count() << "ns" << std::endl;
    std::cout << "Latency P95: " << p95.count() << "ns" << std::endl;
    std::cout << "Latency P99: " << p99.count() << "ns" << std::endl;
    
    // TODO: Verify latency meets requirements
    // EXPECT_LT(p95.count(), max_acceptable_latency_ns);
}

// Scalability Tests
TEST_F(${CLASS_NAME:-${FILE_NAME^}}PerformanceTest, Scalability) {
    std::vector<size_t> data_sizes = {100, 1000, 10000, 100000};
    
    for (auto size : data_sizes) {
        auto start = std::chrono::high_resolution_clock::now();
        
        // TODO: Perform operation with varying data sizes
        // subject->operationWithSize(size);
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "Size: " << size << ", Time: " << duration.count() << "μs" << std::endl;
        
        // TODO: Verify scalability characteristics
        // Linear: O(n), Logarithmic: O(log n), etc.
    }
}

// Helper function for memory measurement
size_t getCurrentMemoryUsage() {
    // TODO: Implement platform-specific memory usage measurement
    // On Linux: read from /proc/self/status
    // On Windows: use GetProcessMemoryInfo
    return 0;
}

// Run benchmarks
BENCHMARK_MAIN();
EOF
}

# Generate mock file if requested
if [[ "$MOCK_DEPENDENCIES" == true && -n "$CLASS_NAME" ]]; then
cat > "$MOCK_FILE" << EOF
#pragma once

#include <gmock/gmock.h>
#include "$INCLUDE_PATH"

// Mock class for $CLASS_NAME dependencies

$(if [[ -n "$CLASS_NAME" ]]; then
cat << MOCKCLASS
class Mock$CLASS_NAME : public $CLASS_NAME {
public:
$(for func in "${FUNCTIONS[@]}"; do
    if [[ "$func" =~ virtual ]]; then
        func_sig=$(echo "$func" | sed 's/virtual\s*//')
        func_name=$(echo "$func_sig" | sed 's/.*\s\+\([A-Za-z_][A-Za-z0-9_]*\)\s*(.*/\1/')
        if [[ -n "$func_name" && "$func_name" != "$CLASS_NAME" ]]; then
            echo "    MOCK_METHOD($(echo "$func_sig" | sed 's/\s*=\s*0\s*;//'), $func_name, (override));"
        fi
    fi
done)
};
MOCKCLASS
fi)

// Mock factory function
std::unique_ptr<Mock$CLASS_NAME> createMock$CLASS_NAME() {
    return std::make_unique<Mock$CLASS_NAME>();
}
EOF
fi

# Make executable
chmod +x "$0"

echo "✅ Test generation complete!"
echo ""
echo "📋 Generated files:"
echo "   📄 $TEST_FILE"
if [[ "$MOCK_DEPENDENCIES" == true ]]; then
    echo "   🎭 $MOCK_FILE"
fi
echo ""
echo "📋 Next steps:"
echo "1. Review and customize the generated test cases"
echo "2. Implement TODO items with actual test logic"
echo "3. Add specific assertions based on expected behavior"
echo "4. Run tests: ctest --test-dir build -R $(basename "$TEST_FILE" .cpp)"
echo "5. Add to version control: git add $TEST_FILE"
echo ""
echo "💡 Test commands:"
echo "   # Run unit tests"
echo "   ctest --test-dir build --output-on-failure"
echo "   # Run with coverage"
echo "   cmake --build build --target coverage"
EOF