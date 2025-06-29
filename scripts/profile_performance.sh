#!/bin/bash

# Performance Profiling and Monitoring Script
# Comprehensive performance analysis for SyLife

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() { echo -e "${BLUE}[INFO]${NC} $1"; }
print_success() { echo -e "${GREEN}[SUCCESS]${NC} $1"; }
print_warning() { echo -e "${YELLOW}[WARNING]${NC} $1"; }
print_error() { echo -e "${RED}[ERROR]${NC} $1"; }

PROFILE_TYPE="all"
OUTPUT_DIR="profiling_results"
TARGET_EXECUTABLE=""
DURATION=30
ITERATIONS=1000

usage() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -t <type>     Profile type: cpu, memory, all (default: all)"
    echo "  -o <dir>      Output directory (default: profiling_results)"
    echo "  -e <exec>     Target executable to profile"
    echo "  -d <seconds>  Profile duration in seconds (default: 30)"
    echo "  -i <count>    Number of iterations for benchmarks (default: 1000)"
    echo "  -h            Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                                    # Profile all aspects"
    echo "  $0 -t cpu -e build/bin/SyLife        # CPU profiling only"
    echo "  $0 -t memory -d 60                   # Memory profiling for 60s"
    exit 1
}

# Parse command line arguments
while getopts "t:o:e:d:i:h" opt; do
    case $opt in
        t) PROFILE_TYPE="$OPTARG" ;;
        o) OUTPUT_DIR="$OPTARG" ;;
        e) TARGET_EXECUTABLE="$OPTARG" ;;
        d) DURATION="$OPTARG" ;;
        i) ITERATIONS="$OPTARG" ;;
        h) usage ;;
        *) usage ;;
    esac
done

echo "⚡ SyLife Performance Profiling"
echo "==============================="

# Create output directory
mkdir -p "$OUTPUT_DIR"
cd "$OUTPUT_DIR"

# Detect available profiling tools
print_status "Detecting profiling tools..."

PERF_AVAILABLE=false
VALGRIND_AVAILABLE=false
GPROF_AVAILABLE=false
HEAPTRACK_AVAILABLE=false
HYPERFINE_AVAILABLE=false

if command -v perf &> /dev/null; then
    PERF_AVAILABLE=true
    print_success "perf detected"
fi

if command -v valgrind &> /dev/null; then
    VALGRIND_AVAILABLE=true
    print_success "valgrind detected"
fi

if command -v gprof &> /dev/null; then
    GPROF_AVAILABLE=true
    print_success "gprof detected"
fi

if command -v heaptrack &> /dev/null; then
    HEAPTRACK_AVAILABLE=true
    print_success "heaptrack detected"
fi

if command -v hyperfine &> /dev/null; then
    HYPERFINE_AVAILABLE=true
    print_success "hyperfine detected"
fi

# Find target executable if not specified
if [[ -z "$TARGET_EXECUTABLE" ]]; then
    if [[ -f "../build/bin/SyLife" ]]; then
        TARGET_EXECUTABLE="../build/bin/SyLife"
    elif [[ -f "../build-modular/bin/modular_example" ]]; then
        TARGET_EXECUTABLE="../build-modular/bin/modular_example"
    elif [[ -f "../build-optimized/bin/SyLife" ]]; then
        TARGET_EXECUTABLE="../build-optimized/bin/SyLife"
    else
        print_error "No target executable found. Please specify with -e"
        exit 1
    fi
fi

print_status "Target executable: $TARGET_EXECUTABLE"

# Function to run CPU profiling
run_cpu_profiling() {
    print_status "Running CPU profiling..."
    
    # perf profiling
    if [[ "$PERF_AVAILABLE" == true ]]; then
        print_status "Running perf analysis..."
        
        # CPU cycles and instructions
        perf stat -e cycles,instructions,cache-references,cache-misses,branches,branch-misses \
            -o perf_stat.txt "$TARGET_EXECUTABLE" &
        PERF_PID=$!
        
        sleep "$DURATION"
        kill -TERM $PERF_PID 2>/dev/null || true
        wait $PERF_PID 2>/dev/null || true
        
        # CPU hotspots
        timeout "$DURATION" perf record -g "$TARGET_EXECUTABLE" || true
        perf report --stdio > perf_report.txt 2>/dev/null || true
        
        print_success "perf analysis complete"
    fi
    
    # gprof profiling (if executable was compiled with -pg)
    if [[ "$GPROF_AVAILABLE" == true ]]; then
        print_status "Running gprof analysis..."
        
        timeout "$DURATION" "$TARGET_EXECUTABLE" || true
        if [[ -f "gmon.out" ]]; then
            gprof "$TARGET_EXECUTABLE" gmon.out > gprof_report.txt
            print_success "gprof analysis complete"
        else
            print_warning "gprof: executable not compiled with -pg flag"
        fi
    fi
}

# Function to run memory profiling
run_memory_profiling() {
    print_status "Running memory profiling..."
    
    # Valgrind memory analysis
    if [[ "$VALGRIND_AVAILABLE" == true ]]; then
        print_status "Running valgrind memory analysis..."
        
        # Memory errors and leaks
        timeout "$DURATION" valgrind --tool=memcheck \
            --leak-check=full \
            --show-leak-kinds=all \
            --track-origins=yes \
            --xml=yes \
            --xml-file=valgrind_memcheck.xml \
            "$TARGET_EXECUTABLE" > valgrind_memcheck.log 2>&1 || true
        
        # Cache profiling
        timeout "$DURATION" valgrind --tool=cachegrind \
            --cachegrind-out-file=cachegrind.out \
            "$TARGET_EXECUTABLE" > valgrind_cachegrind.log 2>&1 || true
        
        if [[ -f "cachegrind.out" ]]; then
            cg_annotate cachegrind.out > cachegrind_report.txt
        fi
        
        # Heap profiling
        timeout "$DURATION" valgrind --tool=massif \
            --massif-out-file=massif.out \
            "$TARGET_EXECUTABLE" > valgrind_massif.log 2>&1 || true
        
        if [[ -f "massif.out" ]] && command -v ms_print &> /dev/null; then
            ms_print massif.out > massif_report.txt
        fi
        
        print_success "valgrind analysis complete"
    fi
    
    # Heaptrack profiling
    if [[ "$HEAPTRACK_AVAILABLE" == true ]]; then
        print_status "Running heaptrack analysis..."
        
        timeout "$DURATION" heaptrack "$TARGET_EXECUTABLE" || true
        
        # Find the heaptrack data file
        HEAPTRACK_FILE=$(ls heaptrack.*.gz 2>/dev/null | head -1)
        if [[ -n "$HEAPTRACK_FILE" ]]; then
            heaptrack_print "$HEAPTRACK_FILE" > heaptrack_report.txt
            print_success "heaptrack analysis complete"
        fi
    fi
}

# Function to run benchmark tests
run_benchmarks() {
    print_status "Running benchmark tests..."
    
    # Built-in benchmarks
    if [[ -f "../build/bin/sylife_benchmarks" ]]; then
        print_status "Running built-in benchmarks..."
        ../build/bin/sylife_benchmarks \
            --benchmark_format=json \
            --benchmark_out=builtin_benchmarks.json \
            --benchmark_repetitions=3
        print_success "Built-in benchmarks complete"
    elif [[ -f "../build-modular/bin/sylife_logger_perf_test" ]]; then
        print_status "Running logger performance test..."
        timeout "$DURATION" ../build-modular/bin/sylife_logger_perf_test > logger_perf.log || true
        print_success "Logger performance test complete"
    fi
    
    # Hyperfine benchmarking
    if [[ "$HYPERFINE_AVAILABLE" == true ]]; then
        print_status "Running hyperfine benchmarks..."
        
        # Quick startup benchmark
        hyperfine --warmup 3 --min-runs 10 \
            --export-json startup_benchmark.json \
            "$TARGET_EXECUTABLE --version || timeout 5s $TARGET_EXECUTABLE" \
            2>/dev/null || true
        
        print_success "hyperfine benchmarks complete"
    fi
    
    # Custom performance tests
    print_status "Running custom performance tests..."
    
    cat > custom_perf_test.cpp << 'EOF'
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>

struct PerformanceTest {
    std::string name;
    std::function<void()> test;
    std::chrono::nanoseconds duration{0};
};

void runPerformanceTests() {
    std::vector<PerformanceTest> tests;
    
    // Memory allocation test
    tests.push_back({"Memory Allocation", []() {
        std::vector<std::unique_ptr<int>> ptrs;
        for (int i = 0; i < 10000; ++i) {
            ptrs.push_back(std::make_unique<int>(i));
        }
    }});
    
    // Vector operations test
    tests.push_back({"Vector Operations", []() {
        std::vector<int> vec(100000);
        std::iota(vec.begin(), vec.end(), 0);
        std::sort(vec.begin(), vec.end(), std::greater<int>());
    }});
    
    // Random access test
    tests.push_back({"Random Access", []() {
        std::vector<int> vec(100000);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 99999);
        
        int sum = 0;
        for (int i = 0; i < 10000; ++i) {
            sum += vec[dis(gen)];
        }
        volatile int result = sum; // Prevent optimization
        (void)result;
    }});
    
    // Run tests
    for (auto& test : tests) {
        auto start = std::chrono::high_resolution_clock::now();
        test.test();
        auto end = std::chrono::high_resolution_clock::now();
        test.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        
        std::cout << test.name << ": " << test.duration.count() << " ns\n";
    }
}

int main() {
    runPerformanceTests();
    return 0;
}
EOF
    
    # Compile and run custom tests
    if g++ -O3 -std=c++20 custom_perf_test.cpp -o custom_perf_test 2>/dev/null; then
        ./custom_perf_test > custom_perf_results.txt
        print_success "Custom performance tests complete"
    else
        print_warning "Could not compile custom performance tests"
    fi
}

# Function to generate performance report
generate_report() {
    print_status "Generating performance report..."
    
    cat > performance_report.md << EOF
# SyLife Performance Analysis Report

Generated: $(date)
Target: $TARGET_EXECUTABLE
Duration: ${DURATION}s
Iterations: $ITERATIONS

## System Information

- **OS**: $(uname -s) $(uname -r)
- **Architecture**: $(uname -m)
- **CPU**: $(lscpu | grep "Model name" | cut -d: -f2 | xargs || echo "Unknown")
- **CPU Cores**: $(nproc)
- **Memory**: $(free -h | awk '/^Mem:/ {print $2}')

## Performance Summary

EOF
    
    # Add perf results if available
    if [[ -f "perf_stat.txt" ]]; then
        echo "### CPU Performance (perf)" >> performance_report.md
        echo '```' >> performance_report.md
        cat perf_stat.txt >> performance_report.md
        echo '```' >> performance_report.md
        echo "" >> performance_report.md
    fi
    
    # Add memory results if available
    if [[ -f "valgrind_memcheck.log" ]]; then
        echo "### Memory Analysis (valgrind)" >> performance_report.md
        echo '```' >> performance_report.md
        tail -20 valgrind_memcheck.log >> performance_report.md
        echo '```' >> performance_report.md
        echo "" >> performance_report.md
    fi
    
    # Add benchmark results if available
    if [[ -f "builtin_benchmarks.json" ]]; then
        echo "### Benchmark Results" >> performance_report.md
        echo '```json' >> performance_report.md
        head -50 builtin_benchmarks.json >> performance_report.md
        echo '```' >> performance_report.md
        echo "" >> performance_report.md
    fi
    
    # Add custom performance results
    if [[ -f "custom_perf_results.txt" ]]; then
        echo "### Custom Performance Tests" >> performance_report.md
        echo '```' >> performance_report.md
        cat custom_perf_results.txt >> performance_report.md
        echo '```' >> performance_report.md
        echo "" >> performance_report.md
    fi
    
    # Add recommendations
    cat >> performance_report.md << EOF
## Recommendations

### CPU Optimization
- Use compiler optimizations (-O3, -march=native)
- Consider SIMD operations for vectorizable code
- Profile hot paths and optimize critical sections

### Memory Optimization
- Use memory pools for frequent allocations
- Minimize memory fragmentation
- Consider cache-friendly data structures

### General Optimization
- Profile before optimizing
- Focus on algorithmic improvements
- Use appropriate data structures
- Consider parallel processing opportunities

## Files Generated

EOF
    
    # List all generated files
    for file in *.txt *.log *.json *.xml *.out; do
        if [[ -f "$file" ]]; then
            echo "- $file" >> performance_report.md
        fi
    done
    
    print_success "Performance report generated: performance_report.md"
}

# Run profiling based on type
case "$PROFILE_TYPE" in
    "cpu")
        run_cpu_profiling
        ;;
    "memory")
        run_memory_profiling
        ;;
    "benchmarks")
        run_benchmarks
        ;;
    "all")
        run_cpu_profiling
        run_memory_profiling
        run_benchmarks
        ;;
    *)
        print_error "Unknown profile type: $PROFILE_TYPE"
        exit 1
        ;;
esac

# Generate final report
generate_report

# Return to original directory
cd ..

echo ""
echo "========================================"
print_success "🎯 Performance Profiling Complete!"
echo "========================================"
echo ""
echo "📊 Results Directory: $OUTPUT_DIR"
echo "📄 Summary Report: $OUTPUT_DIR/performance_report.md"
echo ""
echo "📋 Generated Files:"
ls -la "$OUTPUT_DIR"/*.{txt,log,json,xml,out,md} 2>/dev/null | awk '{print "   " $9 " (" $5 " bytes)"}' || echo "   Check $OUTPUT_DIR/ for all files"
echo ""
echo "💡 Quick Analysis Commands:"
echo "   cat $OUTPUT_DIR/performance_report.md    # View summary"
echo "   perf report                               # Interactive perf analysis"
echo "   valgrind --tool=callgrind $TARGET_EXECUTABLE  # Detailed call analysis"
echo ""
print_success "Ready for performance optimization! ⚡"