# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SyLife is a modern self-designed ecosystem simulator (セルフデザイン生態系シミュレータ) built with C++20 and OpenSiv3D v6.5.0. Users can design their own organisms and build/simulate complex ecosystems. The project follows a modular, secure, and performant architecture supporting both desktop and web platforms.

## Quick Start Commands

### Desktop Build (Recommended)
```bash
# Clone and build
git clone --recursive https://github.com/sknjpn/SyLife.git
cd SyLife
mkdir build && cd build

# Configure with modern CMake
cmake .. -DCMAKE_BUILD_TYPE=Release -DSYLIFE_BUILD_DESKTOP=ON -DSYLIFE_BUILD_TESTS=ON

# Build with parallel jobs
cmake --build . --parallel

# Run tests to verify installation
ctest --output-on-failure

# Run the application
./bin/SyLife
```

### Web Build
```bash
# Setup Emscripten
source /path/to/emsdk/emsdk_env.sh

# Configure for web
mkdir build-web && cd build-web
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DSYLIFE_BUILD_WEB=ON -DSYLIFE_BUILD_DESKTOP=OFF

# Build and serve
cmake --build . --parallel
python3 -m http.server 8080 -d web/
```

### Development Build with Full Testing
```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSYLIFE_BUILD_TESTS=ON \
  -DSYLIFE_ENABLE_COVERAGE=ON \
  -DSYLIFE_ENABLE_SANITIZERS=ON \
  -DSYLIFE_ENABLE_LTO=OFF

make -j$(nproc)
make test_core test_security test_integration
```

## Modern Architecture (v2.0)

### Modular Library Structure
```
lib/
├── core/          # Foundation (World, Object, common utilities)
├── assets/        # Asset management and factory system
├── states/        # Runtime state management and serialization
├── physics/       # High-performance physics engine with spatial partitioning
├── parts/         # Organism component system (Nucleus, Wing, Eye, etc.)
├── network/       # Secure WebSocket communication with authentication
├── ui/            # Modern UI framework with viewer hierarchy
└── utils/         # Performance utilities (memory pools, profilers)
```

### SOLID Principles Implementation
- **Single Responsibility**: Each class has one clear purpose
- **Open/Closed**: Extensible via interfaces and factories
- **Liskov Substitution**: Proper inheritance hierarchies
- **Interface Segregation**: Minimal, focused interfaces
- **Dependency Inversion**: Dependency injection throughout

### Security-First Design
- **Input Validation**: All external input sanitized and validated
- **Authentication**: Token-based authentication with configurable security levels
- **Rate Limiting**: Prevents abuse and DoS attacks
- **Secure Communications**: WSS (WebSocket Secure) support
- **Path Traversal Protection**: Prevents malicious file access
- **Memory Safety**: RAII, smart pointers, bounds checking

## Testing Infrastructure (Comprehensive)

### Test Framework
- **Google Test + Google Mock**: Complete testing framework
- **Coverage Analysis**: lcov integration for coverage reports
- **Static Analysis**: clang-tidy, cppcheck integration
- **Memory Testing**: Valgrind, AddressSanitizer support
- **Performance Testing**: Dedicated benchmark suite

### Test Categories
```bash
# Core functionality
make test_core          # Object system, World management
make test_assets        # Asset management and validation
make test_physics       # Physics engine and collision detection

# Network and security
make test_network       # WebSocket communication
make test_security      # Security manager, rate limiting, input validation

# Integration and performance
make test_integration   # Full simulation integration tests
make benchmark          # Performance benchmarks
make stress            # Stress testing (100 iterations)

# Coverage and analysis
make coverage          # Generate coverage report
make memtest          # Memory leak detection
```

### Test-Driven Development
1. **Write tests first** for all new features
2. **Minimum 90% code coverage** required for merge
3. **Security tests mandatory** for all input/network code
4. **Performance regression tests** for critical paths
5. **Integration tests** for multi-component features

## Performance Optimization

### Memory Management
- **Object Pools**: Reuse frequently allocated objects
- **Stack Allocators**: Fast temporary allocations
- **Memory Alignment**: Optimized for SIMD operations
- **Smart Pointers**: Automatic memory management

### Profiling and Monitoring
```cpp
// Automatic profiling in debug builds
SYLIFE_PROFILE_SCOPE("UpdatePhysics");
SYLIFE_PROFILE_FUNCTION();

// Memory pool usage
auto entity = SYLIFE_POOL_ACQUIRE(CellState, assetPtr, position);

// Stack allocation for temporary data
SYLIFE_STACK_SCOPE(allocator);
auto tempData = SYLIFE_STACK_ALLOC(allocator, PhysicsData, 1000);
```

### Spatial Partitioning
- **KD-Tree**: O(log n) collision detection
- **Dynamic Updates**: Incremental tree rebuilding
- **Multi-threading**: Parallel physics updates
- **Cache-Friendly**: Memory layout optimization

## Security Guidelines

### Input Validation
```cpp
// Always validate external input
auto result = securityManager->validateWebSocketMessage(message, clientToken);
if (!result.isValid) {
    SYLIFE_LOG_WARNING("Invalid input: " + result.reason);
    return;
}

// Validate file operations
result = securityManager->validateFileOperation("read", filePath, token);
if (result.requiredLevel > clientLevel) {
    throw SecurityViolation("Insufficient privileges");
}
```

### Network Security
- **Rate Limiting**: 100 requests/minute default
- **Token Authentication**: JWT-style tokens with expiration
- **Permission System**: Granular permission management
- **IP Banning**: Automatic banning for malicious behavior
- **Input Sanitization**: All input cleaned and validated

### Asset Security
- **Path Validation**: Prevent directory traversal
- **Asset Signing**: Cryptographic asset verification
- **Sandbox Execution**: Isolated asset processing
- **Resource Limits**: Prevent resource exhaustion attacks

## Common Development Workflows

### Adding New Organism Parts
1. **Design**: Create interface in `include/sylife/parts/part_[name].h`
2. **Implement**: Add implementation in `lib/parts/part_[name].cpp`
3. **Register**: Add to part factory system
4. **Test**: Write comprehensive unit tests
5. **Integrate**: Add integration tests with physics system
6. **Document**: Update API documentation

### Performance Optimization Workflow
1. **Profile**: Use built-in profiler to identify bottlenecks
2. **Measure**: Establish baseline with benchmark tests
3. **Optimize**: Apply optimizations (algorithms, memory, etc.)
4. **Verify**: Ensure functionality unchanged with tests
5. **Benchmark**: Measure improvement with performance tests
6. **Document**: Update performance characteristics

### Security Review Process
1. **Threat Model**: Identify potential attack vectors
2. **Static Analysis**: Run clang-tidy and cppcheck
3. **Dynamic Testing**: Use fuzzing and security test suite
4. **Code Review**: Manual security-focused review
5. **Penetration Testing**: Automated security scanning
6. **Documentation**: Update security documentation

## CI/CD Pipeline

### Automated Testing
- **Multi-platform**: Linux, Windows, macOS
- **Multi-compiler**: GCC, Clang, MSVC
- **Code Coverage**: Automatic coverage reporting
- **Security Scanning**: Automated vulnerability detection
- **Performance Regression**: Automated performance monitoring

### Quality Gates
- **Build Success**: All platforms must build
- **Test Passage**: 100% test passage required
- **Coverage Threshold**: 90% minimum coverage
- **Security Clear**: No high/critical vulnerabilities
- **Performance**: No >5% performance regressions

## Best Practices

### Code Style
- **Modern C++20**: Use latest language features appropriately
- **RAII**: Resource management through constructors/destructors
- **const-correctness**: Proper const usage throughout
- **Exception Safety**: Strong exception guarantee where possible
- **Documentation**: Doxygen-style comments for all public APIs

### Error Handling
```cpp
// Use exceptions for error conditions
SYLIFE_THROW("Invalid asset configuration");

// Use expected/optional for fallible operations
std::optional<Asset> loadAsset(const std::string& path);

// Log appropriately by severity
SYLIFE_LOG_ERROR("Critical system failure");
SYLIFE_LOG_WARNING("Recoverable error condition");
SYLIFE_LOG_INFO("Operational information");
SYLIFE_LOG_DEBUG("Detailed diagnostic information");
```

### Performance Considerations
- **Prefer algorithms**: Use STL algorithms over manual loops
- **Minimize allocations**: Use object pools and stack allocators
- **Cache-friendly**: Consider memory layout in hot paths
- **Profile-guided**: Use profiler data to guide optimizations
- **Batch operations**: Group similar operations together

## Critical Development Guidelines

1. **Security First**: All external input must be validated
2. **Test Everything**: TDD approach with comprehensive coverage
3. **Performance Awareness**: Profile before optimizing
4. **Memory Safety**: Use RAII and smart pointers
5. **Cross-platform**: Test on all supported platforms
6. **Documentation**: Keep docs current with code changes
7. **Backward Compatibility**: Maintain API stability
8. **Incremental Development**: Small, testable changes

## Troubleshooting

### Build Issues
- **Missing Dependencies**: Check OpenSiv3D installation
- **Compiler Errors**: Ensure C++20 support enabled
- **Linker Errors**: Verify all required libraries linked

### Runtime Issues
- **Performance**: Use built-in profiler to identify bottlenecks
- **Memory Leaks**: Run with valgrind or AddressSanitizer
- **Security**: Check security manager logs for violations
- **Network**: Verify WebSocket configuration and certificates

### Testing Issues
- **Test Failures**: Check test logs for specific failure reasons
- **Coverage**: Use coverage reports to identify untested code
- **Performance**: Compare benchmark results with baselines

This architecture provides a solid foundation for building complex ecosystem simulations while maintaining high performance, security, and code quality standards.