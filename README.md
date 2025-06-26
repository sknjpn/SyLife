# SyLife

**A Self-Designed Ecosystem Simulator** / セルフデザイン生態系シミュレータ

[![CI](https://github.com/sknjpn/SyLife/workflows/CI/badge.svg)](https://github.com/sknjpn/SyLife/actions)
[![codecov](https://codecov.io/gh/sknjpn/SyLife/branch/main/graph/badge.svg)](https://codecov.io/gh/sknjpn/SyLife)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)

## Overview

SyLife is an advanced ecosystem simulation platform that allows users to design their own organisms and observe complex emergent behaviors in a virtual environment. Built with modern C++20 and powered by OpenSiv3D, it provides both desktop and web experiences for biological simulation and research.

### Key Features

- **🧬 Custom Organism Design**: Create unique organisms with different parts, behaviors, and characteristics
- **🌍 Dynamic Ecosystem**: Simulate complex interactions between organisms, environment, and resources
- **🔬 Scientific Accuracy**: Physics-based simulation with realistic biological processes
- **🖥️ Cross-Platform**: Native desktop application and web browser support
- **📱 Touch Interface**: Optimized for both traditional and touch-based interactions
- **🔧 Modular Architecture**: Extensible design allowing for easy addition of new features
- **📊 Real-time Analytics**: Monitor population dynamics, resource flows, and ecosystem health
- **🎮 Interactive Visualization**: Intuitive interface for designing and observing simulations

## Quick Start

### Prerequisites

- **C++20 compatible compiler** (GCC 11+, Clang 13+, MSVC 2022+)
- **CMake 3.16 or higher**
- **OpenSiv3D v6.5.0** - Download from [official website](https://siv3d.github.io/)
- **Git** with LFS support

### Building from Source

#### Desktop Application (Linux/Windows/macOS)

```bash
# Clone the repository
git clone --recursive https://github.com/sknjpn/SyLife.git
cd SyLife

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release -DSYLIFE_BUILD_DESKTOP=ON

# Build
cmake --build . --parallel

# Run tests (optional)
ctest --output-on-failure

# Install (optional)
cmake --install . --prefix /usr/local
```

#### Web Application

```bash
# Install Emscripten SDK
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install 3.1.20
./emsdk activate 3.1.20
source ./emsdk_env.sh

# Configure for web
mkdir build-web && cd build-web
emcmake cmake .. -DCMAKE_BUILD_TYPE=Release -DSYLIFE_BUILD_WEB=ON -DSYLIFE_BUILD_DESKTOP=OFF

# Build
cmake --build . --parallel

# Serve the web application
python3 -m http.server 8080 -d web/
```

### Development Setup

For development with testing and debugging enabled:

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DSYLIFE_BUILD_TESTS=ON \
  -DSYLIFE_ENABLE_COVERAGE=ON \
  -DSYLIFE_ENABLE_SANITIZERS=ON
```

## Architecture

SyLife follows a modular, component-based architecture:

```
SyLife/
├── lib/               # Core libraries
│   ├── core/         # Fundamental systems (World, Object)
│   ├── assets/       # Asset management (Cell, Part, Protein assets)
│   ├── states/       # Runtime states (Cell, Egg, Part states)
│   ├── physics/      # Physics simulation (Rigidbody, Collision)
│   ├── parts/        # Organism parts (Nucleus, Wing, Eye, etc.)
│   ├── network/      # WebSocket communication
│   ├── ui/           # User interface components
│   └── utils/        # Utility functions and helpers
├── apps/
│   ├── desktop/      # Desktop application
│   └── web/          # Web application
├── tests/            # Comprehensive test suite
├── docs/             # Documentation
└── examples/         # Example projects and tutorials
```

### Key Components

- **World**: Central simulation environment managing all entities and subsystems
- **Assets**: Template definitions for organisms, parts, and proteins
- **States**: Runtime instances of assets with dynamic properties
- **Physics Engine**: Custom 2D physics with spatial partitioning for performance
- **Part System**: Modular organism components (Body, Nucleus, Wings, Eyes, etc.)
- **Network Layer**: Real-time multiplayer and remote monitoring capabilities

## Usage Examples

### Creating Your First Organism

```cpp
#include <sylife/sylife.h>

using namespace sylife;

int main() {
    // Initialize world
    auto world = core::World::getInstance();
    world->initialize(100, 100);
    
    // Create a simple cell
    auto cellAsset = world->getAssetManager().create<assets::CellAsset>();
    cellAsset->setName("MyFirstCell");
    cellAsset->setMass(1.0);
    cellAsset->setRadius(2.0);
    
    // Add basic parts
    auto nucleus = cellAsset->addPart<parts::Nucleus>();
    auto wing = cellAsset->addPart<parts::Wing>();
    
    // Spawn in world
    auto cellState = world->getStateManager().createCell(cellAsset, {50, 50});
    
    // Run simulation
    world->run();
    
    return 0;
}
```

### Advanced Ecosystem Setup

See the [examples/](examples/) directory for comprehensive tutorials including:
- Predator-prey ecosystems
- Resource competition models
- Evolutionary algorithms
- Multi-species interactions

## Documentation

- **[API Reference](docs/api/)** - Complete API documentation
- **[User Guide](docs/guide/)** - Step-by-step tutorials and examples
- **[Architecture Guide](docs/architecture/)** - Internal design and extension points
- **[Contributing](CONTRIBUTING.md)** - Development guidelines and contribution process

## Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Linux (Ubuntu 20.04+) | ✅ Fully Supported | Primary development platform |
| Windows 10/11 | ✅ Fully Supported | Visual Studio 2022+ |
| macOS 11+ | ✅ Fully Supported | Intel and Apple Silicon |
| Web (Chrome, Firefox, Safari) | ✅ Fully Supported | WebAssembly + WebGL2 |
| Mobile | 🚧 Planned | Touch-optimized interface |

## Performance

SyLife is optimized for real-time simulation of complex ecosystems:

- **Spatial Partitioning**: O(log n) collision detection using KD-trees
- **Multi-threading**: Parallel physics and logic updates
- **Memory Pool**: Efficient memory management for dynamic entities
- **GPU Acceleration**: WebGL/OpenGL rendering with batching
- **Scalability**: Handle 1000+ organisms at 60 FPS

## Contributing

We welcome contributions! Please see our [Contributing Guide](CONTRIBUTING.md) for details.

### Development Workflow

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Write tests for your changes
4. Ensure all tests pass (`cmake --build . && ctest`)
5. Run static analysis (`clang-tidy`, `cppcheck`)
6. Format code (`clang-format`)
7. Commit changes (`git commit -m 'Add amazing feature'`)
8. Push to branch (`git push origin feature/amazing-feature`)
9. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **OpenSiv3D Team** - For the excellent graphics and audio framework
- **Scientific Community** - For biological simulation research and inspiration
- **Contributors** - Everyone who has contributed to making SyLife better

## Contact

- **Project Homepage**: https://github.com/sknjpn/SyLife
- **Issue Tracker**: https://github.com/sknjpn/SyLife/issues
- **Discussions**: https://github.com/sknjpn/SyLife/discussions

---

*Built with ❤️ for the scientific simulation community*
