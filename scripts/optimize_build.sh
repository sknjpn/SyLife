#!/bin/bash

# Build Optimization Script
# Automatically configures optimal build settings for the current system

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

BUILD_TYPE="Release"
BUILD_DIR="build-optimized"
CLEAN_BUILD=false
PROFILE_BUILD=false
BENCHMARK_BUILD=false

usage() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -t <type>     Build type: Debug, Release, RelWithDebInfo (default: Release)"
    echo "  -d <dir>      Build directory (default: build-optimized)"
    echo "  -c            Clean build (remove existing build directory)"
    echo "  -p            Enable build time profiling"
    echo "  -b            Run benchmarks after build"
    echo "  -h            Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                           # Optimized release build"
    echo "  $0 -t Debug -c              # Clean debug build"
    echo "  $0 -p -b                     # Profile build and run benchmarks"
    exit 1
}

# Parse command line arguments
while getopts "t:d:cpbh" opt; do
    case $opt in
        t) BUILD_TYPE="$OPTARG" ;;
        d) BUILD_DIR="$OPTARG" ;;
        c) CLEAN_BUILD=true ;;
        p) PROFILE_BUILD=true ;;
        b) BENCHMARK_BUILD=true ;;
        h) usage ;;
        *) usage ;;
    esac
done

echo "🚀 SyLife Build Optimization"
echo "============================"

# Detect system capabilities
print_status "Detecting system capabilities..."

# CPU information
CPU_CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo "4")
CPU_ARCH=$(uname -m)
TOTAL_RAM=$(free -h 2>/dev/null | awk '/^Mem:/ {print $2}' || echo "Unknown")

print_status "CPU: $CPU_ARCH ($CPU_CORES cores)"
print_status "RAM: $TOTAL_RAM"

# Compiler detection and optimization
print_status "Detecting compiler capabilities..."

# Check for compilers
GCC_VERSION=""
CLANG_VERSION=""
MSVC_VERSION=""

if command -v gcc &> /dev/null; then
    GCC_VERSION=$(gcc --version | head -1 | grep -oE '[0-9]+\.[0-9]+')
    print_status "GCC: $GCC_VERSION"
fi

if command -v clang++ &> /dev/null; then
    CLANG_VERSION=$(clang++ --version | head -1 | grep -oE '[0-9]+\.[0-9]+')
    print_status "Clang: $CLANG_VERSION"
fi

# Choose optimal compiler
PREFERRED_COMPILER=""
if [[ -n "$CLANG_VERSION" ]] && [[ $(echo "$CLANG_VERSION >= 12.0" | bc -l 2>/dev/null || echo "1") -eq 1 ]]; then
    PREFERRED_COMPILER="clang++"
    print_success "Using Clang++ for optimal performance"
elif [[ -n "$GCC_VERSION" ]] && [[ $(echo "$GCC_VERSION >= 11.0" | bc -l 2>/dev/null || echo "1") -eq 1 ]]; then
    PREFERRED_COMPILER="g++"
    print_success "Using GCC for optimal performance"
else
    print_warning "Using default compiler (may not be optimal)"
fi

# Detect build tools
print_status "Detecting build tools..."

NINJA_AVAILABLE=false
CCACHE_AVAILABLE=false
LLD_AVAILABLE=false

if command -v ninja &> /dev/null; then
    NINJA_AVAILABLE=true
    print_success "Ninja build system detected"
fi

if command -v ccache &> /dev/null; then
    CCACHE_AVAILABLE=true
    print_success "ccache detected"
fi

if command -v lld &> /dev/null; then
    LLD_AVAILABLE=true
    print_success "LLD linker detected"
fi

# Clean build if requested
if [[ "$CLEAN_BUILD" == true ]]; then
    print_status "Cleaning previous build..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure CMake with optimization settings
print_status "Configuring optimized build..."

CMAKE_ARGS=(
    ".."
    "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
    "-DSYLIFE_BUILD_TESTS=ON"
    "-DSYLIFE_BUILD_BENCHMARKS=ON"
    "-DSYLIFE_ENABLE_LTO=ON"
    "-DSYLIFE_ENABLE_PCH=ON"
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
)

# Add compiler-specific optimizations
if [[ -n "$PREFERRED_COMPILER" ]]; then
    CMAKE_ARGS+=("-DCMAKE_CXX_COMPILER=$PREFERRED_COMPILER")
fi

# Use Ninja if available
if [[ "$NINJA_AVAILABLE" == true ]]; then
    CMAKE_ARGS+=("-GNinja")
    print_status "Using Ninja generator for faster builds"
fi

# Enable ccache if available
if [[ "$CCACHE_AVAILABLE" == true ]]; then
    CMAKE_ARGS+=("-DSYLIFE_ENABLE_CCACHE=ON")
    print_status "Using ccache for build acceleration"
fi

# Enable LLD linker if available and using Clang
if [[ "$LLD_AVAILABLE" == true ]] && [[ "$PREFERRED_COMPILER" == "clang++" ]]; then
    CMAKE_ARGS+=("-DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld")
    CMAKE_ARGS+=("-DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld")
    print_status "Using LLD linker for faster linking"
fi

# Build type specific optimizations
case "$BUILD_TYPE" in
    "Release")
        CMAKE_ARGS+=("-DSYLIFE_ENABLE_UNITY_BUILD=ON")
        print_status "Release build with unity build enabled"
        ;;
    "Debug")
        CMAKE_ARGS+=("-DSYLIFE_ENABLE_SANITIZERS=ON")
        CMAKE_ARGS+=("-DSYLIFE_ENABLE_COVERAGE=ON")
        print_status "Debug build with sanitizers and coverage"
        ;;
    "RelWithDebInfo")
        CMAKE_ARGS+=("-DSYLIFE_ENABLE_LTO=OFF")  # LTO can interfere with debug info
        print_status "Release with debug info build"
        ;;
esac

# Enable build profiling if requested
if [[ "$PROFILE_BUILD" == true ]]; then
    CMAKE_ARGS+=("-DSYLIFE_PROFILE_BUILD_TIME=ON")
    print_status "Build time profiling enabled"
fi

# Configure
print_status "Running CMake configuration..."
cmake "${CMAKE_ARGS[@]}"

# Calculate optimal parallel jobs
# Use fewer jobs if system has limited RAM to avoid OOM
MEMORY_GB=$(free -g 2>/dev/null | awk '/^Mem:/ {print $2}' || echo "8")
OPTIMAL_JOBS=$CPU_CORES

# Adjust jobs based on available memory (rough heuristic: 2GB per job for C++)
if [[ "$MEMORY_GB" -lt $((OPTIMAL_JOBS * 2)) ]]; then
    OPTIMAL_JOBS=$((MEMORY_GB / 2))
    if [[ "$OPTIMAL_JOBS" -lt 1 ]]; then
        OPTIMAL_JOBS=1
    fi
    print_warning "Reducing parallel jobs to $OPTIMAL_JOBS due to limited memory"
fi

print_status "Using $OPTIMAL_JOBS parallel jobs"

# Build
print_status "Building project..."
start_time=$(date +%s)

if [[ "$NINJA_AVAILABLE" == true ]]; then
    ninja -j$OPTIMAL_JOBS
else
    make -j$OPTIMAL_JOBS
fi

end_time=$(date +%s)
build_duration=$((end_time - start_time))

print_success "Build completed in ${build_duration} seconds"

# Run tests
print_status "Running tests..."
ctest --output-on-failure --parallel $OPTIMAL_JOBS

# Generate build reports
print_status "Generating build reports..."

# Compilation database for IDE integration
if [[ -f "compile_commands.json" ]]; then
    cp compile_commands.json ../
    print_success "Compilation database copied to project root"
fi

# Build statistics
echo "Build Statistics:" > build_stats.txt
echo "=================" >> build_stats.txt
echo "Build Type: $BUILD_TYPE" >> build_stats.txt
echo "Build Duration: ${build_duration}s" >> build_stats.txt
echo "Parallel Jobs: $OPTIMAL_JOBS" >> build_stats.txt
echo "Compiler: $PREFERRED_COMPILER" >> build_stats.txt
echo "Generator: $(cmake --help | head -1)" >> build_stats.txt
echo "Date: $(date)" >> build_stats.txt

# Count build artifacts
echo "" >> build_stats.txt
echo "Build Artifacts:" >> build_stats.txt
find . -name "*.o" | wc -l | awk '{print "Object files: " $1}' >> build_stats.txt
find . -name "*.a" | wc -l | awk '{print "Static libraries: " $1}' >> build_stats.txt
find . -name "*.so" | wc -l | awk '{print "Shared libraries: " $1}' >> build_stats.txt
find . -type f -executable | wc -l | awk '{print "Executables: " $1}' >> build_stats.txt

print_success "Build statistics saved to build_stats.txt"

# Generate optimization report
if [[ "$PROFILE_BUILD" == true ]]; then
    print_status "Generating optimization report..."
    
    # Clang time-trace analysis
    if [[ "$PREFERRED_COMPILER" == "clang++" ]] && find . -name "*.json" | grep -q .; then
        echo "Top compilation time consumers:" > optimization_report.txt
        find . -name "*.json" -exec cat {} \; | \
        grep -E '"dur":\s*[0-9]+' | \
        sort -t: -k2 -nr | \
        head -20 >> optimization_report.txt
        
        print_success "Optimization report saved to optimization_report.txt"
    fi
fi

# Run benchmarks if requested
if [[ "$BENCHMARK_BUILD" == true ]]; then
    print_status "Running benchmarks..."
    
    if [[ -f "bin/sylife_benchmarks" ]]; then
        ./bin/sylife_benchmarks --benchmark_format=json --benchmark_out=benchmarks.json
        print_success "Benchmark results saved to benchmarks.json"
    else
        print_warning "Benchmark executable not found"
    fi
fi

# ccache statistics
if [[ "$CCACHE_AVAILABLE" == true ]]; then
    print_status "ccache statistics:"
    ccache -s
fi

# Final summary
cd ..
echo ""
echo "=========================================="
print_success "🎉 Optimized Build Complete!"
echo "=========================================="
echo ""
echo "📊 Build Summary:"
echo "   Duration: ${build_duration}s"
echo "   Jobs: $OPTIMAL_JOBS"
echo "   Compiler: $PREFERRED_COMPILER"
echo "   Type: $BUILD_TYPE"
echo ""
echo "📁 Build Directory: $BUILD_DIR"
echo "📄 Build Stats: $BUILD_DIR/build_stats.txt"
if [[ "$PROFILE_BUILD" == true ]]; then
    echo "📈 Optimization Report: $BUILD_DIR/optimization_report.txt"
fi
if [[ "$BENCHMARK_BUILD" == true ]]; then
    echo "⚡ Benchmarks: $BUILD_DIR/benchmarks.json"
fi
echo ""
echo "🚀 Quick Commands:"
echo "   cd $BUILD_DIR && ninja           # Incremental build"
echo "   cd $BUILD_DIR && ctest           # Run tests"
echo "   ./scripts/run_tests.sh           # Full test suite"
echo ""
print_success "Ready for high-performance development! 🚀"