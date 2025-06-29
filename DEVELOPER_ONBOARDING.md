# SyLife Developer Onboarding Guide

Welcome to the SyLife project! This guide will help you get up and running quickly with our modern C++20 ecosystem simulator.

## 🚀 Quick Start (5 minutes)

### Prerequisites
- C++20 compatible compiler (GCC 11+, Clang 12+, MSVC 2022+)
- CMake 3.20+
- Git
- VSCode (recommended)

### One-Command Setup
```bash
# Clone and setup everything
git clone --recursive https://github.com/sknjpn/SyLife.git
cd SyLife
./scripts/setup_development_environment.sh
```

### Verify Installation
```bash
# Build and run tests
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSYLIFE_BUILD_TESTS=ON
cmake --build . --parallel
ctest --output-on-failure

# Run example
./bin/modular_example
```

If you see colored log output, you're ready to go! 🎉

## 📁 Project Structure Overview

```
SyLife/
├── 🏗️  modules/           # Core modular architecture
│   ├── core/              # Foundation interfaces
│   ├── di/               # Dependency injection
│   ├── logger/           # Logging system
│   └── ecs/              # Entity Component System
├── 🎮  src/              # Main application code
├── 🧪  tests/            # Test suite
├── 🔧  scripts/          # Development tools
├── 🐳  docker/           # Container environments
└── 📚  docs/             # Documentation
```

## 🛠️ Development Workflow

### 1. Feature Development
```bash
# Create feature branch
git checkout -b feature/my-awesome-feature

# Generate component (if needed)
./scripts/generate_component.sh -n MyComponent -m ecs -f "float:x,float:y"

# Generate system (if needed)
./scripts/generate_system.sh -n MySystem -m ecs -c "MyComponent"

# Write tests first (TDD)
./scripts/generate_test.sh -s modules/ecs/src/my_component.cpp

# Build and test
cmake --build build --parallel
ctest --test-dir build
```

### 2. Code Quality Checks
```bash
# Automatic on commit (pre-commit hooks)
git commit -m "feat: add awesome feature"

# Manual quality check
./scripts/run_quality_checks.sh
```

### 3. Debugging
```bash
# Quick debug in VSCode
# F5 -> Select "🚀 Debug SyLife (Main Application)"

# Debug with GDB
gdb build/bin/SyLife
(gdb) run
```

## 🎯 Development Principles

### 1. Test-Driven Development (TDD)
```cpp
// 1. Write test first
TEST(MyComponentTest, ShouldDoSomething) {
    MyComponent component;
    EXPECT_EQ(component.getValue(), 42);
}

// 2. Make test pass
class MyComponent {
public:
    int getValue() const { return 42; }
};

// 3. Refactor
```

### 2. Modern C++20 Patterns
```cpp
// Use concepts, ranges, coroutines
template<EntityComponent T>
void processComponent(T&& component) {
    // Type-safe component processing
}

// RAII everywhere
class ResourceManager {
    std::unique_ptr<Resource> resource_;
public:
    ResourceManager() : resource_(std::make_unique<Resource>()) {}
    // Automatic cleanup
};
```

### 3. ECS Architecture
```cpp
// Create entity
auto entity = world.createEntity();

// Add components
world.addComponent<PositionComponent>(entity, {10.0f, 20.0f});
world.addComponent<VelocityComponent>(entity, {1.0f, 0.0f});

// Systems process automatically
world.update(deltaTime);
```

## 🔧 Essential Tools

### VSCode Extensions (Auto-installed)
- **C/C++**: IntelliSense and debugging
- **CMake Tools**: Project management
- **GitLens**: Git visualization
- **Error Lens**: Inline error display
- **TODO Tree**: Task tracking

### Useful Commands
```bash
# Format all code
./scripts/format_code.sh

# Run all tests with coverage
./scripts/run_tests.sh

# Generate documentation
./scripts/generate_docs.sh

# Profile performance
./scripts/profile_performance.sh
```

## 🚨 Common Issues & Solutions

### Build Issues
```bash
# Clear CMake cache
rm -rf build && mkdir build

# Update submodules
git submodule update --init --recursive

# Check dependencies
./scripts/check_dependencies.sh
```

### Test Failures
```bash
# Run specific test
ctest --test-dir build -R MyTest -V

# Debug test
gdb build/bin/my_test
```

### Performance Issues
```bash
# Profile with built-in tools
./build/bin/sylife_benchmarks

# Memory analysis
valgrind --tool=memcheck ./build/bin/SyLife
```

## 📊 Code Metrics & Quality

### Quality Gates
- ✅ **Test Coverage**: >90%
- ✅ **Build Time**: <5 minutes
- ✅ **Static Analysis**: 0 warnings
- ✅ **Security Scan**: 0 vulnerabilities

### Monitoring
```bash
# Check metrics
./scripts/check_metrics.sh

# View coverage report
open build/coverage/index.html

# Performance dashboard
./scripts/performance_dashboard.sh
```

## 🎨 Coding Standards

### Naming Conventions
```cpp
// Classes: PascalCase
class MyAwesomeClass {};

// Functions: camelCase
void doSomething();

// Variables: snake_case
int my_variable = 42;

// Constants: UPPER_CASE
constexpr int MAX_ENTITIES = 10000;

// Namespaces: snake_case
namespace sylife::ecs {}
```

### File Organization
```cpp
// Header files (.h)
#pragma once

#include <system_headers>
#include "project_headers.h"

namespace sylife::module {
class MyClass {
public:
    // Public interface
private:
    // Implementation details
};
} // namespace sylife::module
```

## 🤝 Collaboration Guidelines

### Git Workflow
```bash
# 1. Create feature branch
git checkout -b feature/description

# 2. Commit with conventional commits
git commit -m "feat(ecs): add position component"
git commit -m "fix(logger): resolve memory leak"
git commit -m "docs(readme): update installation guide"

# 3. Push and create PR
git push origin feature/description
gh pr create --title "Add Position Component" --body "Implements..."
```

### Code Review Checklist
- [ ] Tests written and passing
- [ ] Documentation updated
- [ ] Performance impact considered
- [ ] Security implications reviewed
- [ ] Breaking changes documented

### Communication
- 💬 **Discord**: Daily discussions
- 📧 **Email**: Formal communications
- 🐛 **Issues**: Bug reports and features
- 📝 **Wiki**: Design documents

## 🚀 Advanced Topics

### Performance Optimization
```cpp
// Memory pools for frequent allocations
class EntityManager {
    MemoryPool<Entity> entity_pool_;
public:
    Entity* createEntity() {
        return entity_pool_.acquire();
    }
};

// SIMD operations for bulk processing
void updatePositions(std::span<Position> positions, 
                    std::span<Velocity> velocities, 
                    float deltaTime) {
    // Vectorized processing
}
```

### Debugging Techniques
```cpp
// Conditional compilation for debug builds
#ifdef SYLIFE_DEBUG
    #define SYLIFE_ASSERT(condition) assert(condition)
    #define SYLIFE_DEBUG_LOG(msg) std::cout << msg << std::endl
#else
    #define SYLIFE_ASSERT(condition)
    #define SYLIFE_DEBUG_LOG(msg)
#endif

// Runtime debugging
void debugEntity(Entity entity) {
    auto& world = World::getInstance();
    std::cout << "Entity " << entity.id() << " components:\n";
    world.forEachComponent(entity, [](auto& component) {
        std::cout << "  " << typeid(component).name() << std::endl;
    });
}
```

### Extending the Architecture
```cpp
// Add new component types
class HealthComponent {
    float current_health = 100.0f;
    float max_health = 100.0f;
public:
    // Component interface
};

// Register with factory
ComponentFactory::registerComponent<HealthComponent>("Health");

// Create custom systems
class HealthSystem : public System<HealthComponent, DamageComponent> {
public:
    void update(float deltaTime) override {
        // Process health logic
    }
};
```

## 📚 Learning Resources

### Documentation
- 📖 [Architecture Guide](MODULAR_ARCHITECTURE.md)
- 🔒 [Security Guidelines](SECURITY.md)
- ⚡ [Performance Guide](PERFORMANCE.md)
- 🧪 [Testing Strategy](TESTING.md)

### External Resources
- 📚 **Modern C++**: https://en.cppreference.com/
- 🎮 **ECS Pattern**: https://github.com/skypjack/entt
- 🏗️ **CMake**: https://cmake.org/documentation/
- 🧪 **Google Test**: https://google.github.io/googletest/

### Training Modules
1. **Week 1**: C++20 features and project setup
2. **Week 2**: ECS architecture and component design
3. **Week 3**: System implementation and testing
4. **Week 4**: Performance optimization and debugging

## 🆘 Getting Help

### First Steps
1. 📖 Check this guide and documentation
2. 🔍 Search existing issues
3. 💬 Ask in Discord #development channel
4. 📧 Contact mentors

### Creating Issues
```markdown
## Bug Report
**Environment**: OS, Compiler, CMake version
**Steps to Reproduce**: Detailed steps
**Expected Behavior**: What should happen
**Actual Behavior**: What actually happens
**Additional Info**: Logs, screenshots, etc.
```

### Mentorship Program
- 👨‍🏫 **Senior Developers**: Architecture and design
- 🧪 **Test Engineers**: Testing strategies
- ⚡ **Performance Team**: Optimization techniques
- 🔒 **Security Team**: Secure coding practices

## 🎉 Welcome to the Team!

You're now ready to contribute to SyLife! Remember:
- 🚀 Start small and iterate
- 🧪 Test everything
- 💬 Ask questions
- 🤝 Help others
- 🏆 Have fun building awesome software!

**Next Steps**:
1. Join the Discord server
2. Pick a "good first issue" from GitHub
3. Introduce yourself to the team
4. Start coding!

Happy coding! 🎮✨