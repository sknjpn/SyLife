#!/bin/bash

# SyLife Test Runner Script
# Runs comprehensive test suite with coverage and performance monitoring

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
BUILD_DIR="build-test"
COVERAGE_DIR="coverage"
BENCHMARK_DIR="benchmarks"
REPORTS_DIR="reports"

echo -e "${BLUE}🧪 SyLife Test Runner${NC}"
echo "=================================="

# Create directories
mkdir -p ${BUILD_DIR} ${COVERAGE_DIR} ${BENCHMARK_DIR} ${REPORTS_DIR}

# Function to print status
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Configure CMake for testing
print_status "Configuring build for testing..."
cd ${BUILD_DIR}
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DSYLIFE_BUILD_TESTS=ON \
    -DSYLIFE_ENABLE_COVERAGE=ON \
    -DSYLIFE_ENABLE_SANITIZERS=ON \
    -DSYLIFE_BUILD_BENCHMARKS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -GNinja

if [ $? -eq 0 ]; then
    print_success "CMake configuration completed"
else
    print_error "CMake configuration failed"
    exit 1
fi

# Build the project
print_status "Building project..."
ninja -j$(nproc)

if [ $? -eq 0 ]; then
    print_success "Build completed"
else
    print_error "Build failed"
    exit 1
fi

# Run unit tests
print_status "Running unit tests..."
ctest --output-on-failure --timeout 300 --parallel $(nproc)

if [ $? -eq 0 ]; then
    print_success "All unit tests passed"
else
    print_error "Some unit tests failed"
    exit 1
fi

# Generate coverage report
if command -v gcovr &> /dev/null; then
    print_status "Generating coverage report..."
    gcovr --root .. \
          --html --html-details \
          --output ../${COVERAGE_DIR}/coverage.html \
          --xml-pretty \
          --xml ../${COVERAGE_DIR}/coverage.xml \
          --exclude '../thirdparty/.*' \
          --exclude '../build.*/.*' \
          --exclude '../tests/.*'
    
    # Extract coverage percentage
    COVERAGE_PERCENT=$(gcovr --root .. --exclude '../thirdparty/.*' --exclude '../build.*/.*' --exclude '../tests/.*' | grep TOTAL | awk '{print $4}' | sed 's/%//')
    
    if (( $(echo "$COVERAGE_PERCENT >= 90" | bc -l) )); then
        print_success "Code coverage: ${COVERAGE_PERCENT}% (Target: 90%)"
    else
        print_warning "Code coverage: ${COVERAGE_PERCENT}% (Below target: 90%)"
    fi
else
    print_warning "gcovr not found, skipping coverage report"
fi

# Run benchmarks if available
if [ -f "./bin/sylife_benchmarks" ]; then
    print_status "Running performance benchmarks..."
    ./bin/sylife_benchmarks --benchmark_format=json --benchmark_out=../${BENCHMARK_DIR}/results.json
    print_success "Benchmarks completed"
fi

# Run memory leak detection
if command -v valgrind &> /dev/null && [ -f "./bin/sylife_tests" ]; then
    print_status "Running memory leak detection..."
    valgrind --tool=memcheck \
             --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --xml=yes \
             --xml-file=../${REPORTS_DIR}/memcheck.xml \
             ./bin/sylife_tests > ../${REPORTS_DIR}/memcheck.log 2>&1
    
    if [ $? -eq 0 ]; then
        print_success "Memory leak detection completed"
    else
        print_warning "Memory leak detection found issues (check reports/memcheck.xml)"
    fi
fi

# Run static analysis
cd ..
if command -v clang-tidy &> /dev/null; then
    print_status "Running static analysis..."
    find src modules -name "*.cpp" -o -name "*.h" | \
    xargs clang-tidy -p ${BUILD_DIR} \
                     --config-file=.clang-tidy \
                     --export-fixes=${REPORTS_DIR}/clang-tidy-fixes.yaml
    print_success "Static analysis completed"
fi

# Run security analysis
if command -v cppcheck &> /dev/null; then
    print_status "Running security analysis..."
    cppcheck --enable=all \
             --inconclusive \
             --xml \
             --xml-version=2 \
             --suppressions-list=.cppcheck-suppressions \
             src/ modules/ 2> ${REPORTS_DIR}/cppcheck.xml
    print_success "Security analysis completed"
fi

# Generate test report
print_status "Generating test report..."
cat > ${REPORTS_DIR}/test_summary.md << EOF
# SyLife Test Report

Generated: $(date)

## Test Results
- **Unit Tests**: $(ctest --test-dir ${BUILD_DIR} --list-tests | wc -l) tests
- **Coverage**: ${COVERAGE_PERCENT:-"N/A"}%
- **Build Status**: ✅ PASSED

## Files Generated
- Coverage Report: coverage/coverage.html
- Benchmark Results: benchmarks/results.json
- Memory Check: reports/memcheck.xml
- Static Analysis: reports/clang-tidy-fixes.yaml
- Security Analysis: reports/cppcheck.xml

## Quick Links
- [Coverage Report](coverage/coverage.html)
- [CI/CD Status](../../actions)
EOF

print_success "Test report generated: ${REPORTS_DIR}/test_summary.md"

# Final summary
echo
echo "=================================="
print_success "🎉 All tests completed successfully!"
echo -e "📊 Coverage: ${GREEN}${COVERAGE_PERCENT:-"N/A"}%${NC}"
echo -e "📁 Reports available in: ${BLUE}${REPORTS_DIR}/${NC}"
echo -e "🌐 Coverage report: ${BLUE}${COVERAGE_DIR}/coverage.html${NC}"
echo "=================================="