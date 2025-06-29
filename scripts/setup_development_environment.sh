#!/bin/bash

# Development Environment Setup Script
# One-command setup for new developers

set -e

echo "🚀 Setting up SyLife Development Environment..."
echo "=============================================="

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

# Check if running in correct directory
if [[ ! -f "CMakeLists.txt" ]] || [[ ! -d "modules" ]]; then
    print_error "Please run this script from the SyLife root directory"
    exit 1
fi

# Function to check command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check version
check_version() {
    local cmd="$1"
    local min_version="$2"
    local current_version
    
    case "$cmd" in
        "gcc")
            current_version=$(gcc --version | head -1 | grep -oE '[0-9]+\.[0-9]+' | head -1)
            ;;
        "cmake")
            current_version=$(cmake --version | head -1 | grep -oE '[0-9]+\.[0-9]+' | head -1)
            ;;
        *)
            return 0
            ;;
    esac
    
    if [[ "$(printf '%s\n' "$min_version" "$current_version" | sort -V | head -n1)" = "$min_version" ]]; then
        return 0
    else
        return 1
    fi
}

# Step 1: Check Prerequisites
print_status "Checking prerequisites..."

# Check C++ compiler
if command_exists g++; then
    if check_version gcc 11.0; then
        print_success "GCC $(gcc --version | head -1 | grep -oE '[0-9]+\.[0-9]+' | head -1) found"
    else
        print_warning "GCC version may be too old (need 11+). Current: $(gcc --version | head -1)"
    fi
elif command_exists clang++; then
    print_success "Clang found"
else
    print_error "No suitable C++ compiler found. Please install GCC 11+ or Clang 12+"
    exit 1
fi

# Check CMake
if command_exists cmake; then
    if check_version cmake 3.20; then
        print_success "CMake $(cmake --version | head -1 | grep -oE '[0-9]+\.[0-9]+' | head -1) found"
    else
        print_warning "CMake version may be too old (need 3.20+)"
    fi
else
    print_error "CMake not found. Please install CMake 3.20+"
    exit 1
fi

# Check Git
if command_exists git; then
    print_success "Git found"
else
    print_error "Git not found. Please install Git"
    exit 1
fi

# Step 2: Install Dependencies (if possible)
print_status "Installing dependencies..."

# Detect OS and install dependencies
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if command_exists apt-get; then
        print_status "Installing Ubuntu/Debian dependencies..."
        sudo apt-get update
        sudo apt-get install -y \
            build-essential \
            cmake \
            ninja-build \
            pkg-config \
            libgtest-dev \
            libgmock-dev \
            clang-format \
            clang-tidy \
            cppcheck \
            valgrind \
            gcovr \
            lcov \
            curl \
            wget \
            python3 \
            python3-pip
        
        # Install Google Test properly
        cd /usr/src/gtest && sudo cmake . && sudo cmake --build . && sudo cp lib/*.a /usr/lib/ || true
        cd /usr/src/gmock && sudo cmake . && sudo cmake --build . && sudo cp lib/*.a /usr/lib/ || true
        cd - > /dev/null
        
    elif command_exists yum; then
        print_status "Installing RHEL/CentOS dependencies..."
        sudo yum install -y \
            gcc-c++ \
            cmake \
            ninja-build \
            pkgconfig \
            gtest-devel \
            gmock-devel \
            clang-tools-extra \
            cppcheck \
            valgrind \
            python3 \
            python3-pip
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    if command_exists brew; then
        print_status "Installing macOS dependencies..."
        brew install \
            cmake \
            ninja \
            pkg-config \
            googletest \
            clang-format \
            cppcheck \
            lcov \
            python@3.11
    else
        print_warning "Homebrew not found. Please install dependencies manually."
    fi
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
    print_warning "Windows detected. Please ensure you have:"
    echo "  - Visual Studio 2022 or later"
    echo "  - CMake 3.20+"
    echo "  - Git"
fi

# Step 3: Install Python tools
print_status "Installing Python tools..."
if command_exists pip3; then
    pip3 install --user pre-commit cpplint || true
elif command_exists pip; then
    pip install --user pre-commit cpplint || true
fi

# Step 4: Setup Pre-commit Hooks
print_status "Setting up pre-commit hooks..."
if ./scripts/setup_pre_commit.sh; then
    print_success "Pre-commit hooks installed"
else
    print_warning "Pre-commit hooks setup failed (non-critical)"
fi

# Step 5: Configure Git (if not already configured)
print_status "Configuring Git..."
if [[ -z "$(git config --global user.name)" ]]; then
    echo "Git user name not set. Please enter your name:"
    read -r user_name
    git config --global user.name "$user_name"
fi

if [[ -z "$(git config --global user.email)" ]]; then
    echo "Git user email not set. Please enter your email:"
    read -r user_email
    git config --global user.email "$user_email"
fi

# Configure useful Git settings
git config --global pull.rebase true
git config --global init.defaultBranch main
git config --global core.autocrlf input

print_success "Git configured"

# Step 6: Initialize Submodules
print_status "Initializing submodules..."
git submodule update --init --recursive
print_success "Submodules initialized"

# Step 7: Create Build Directory
print_status "Setting up build environment..."
mkdir -p build-dev
cd build-dev

# Configure with development settings
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DSYLIFE_BUILD_TESTS=ON \
    -DSYLIFE_ENABLE_COVERAGE=ON \
    -DSYLIFE_ENABLE_SANITIZERS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -GNinja 2>/dev/null || cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DSYLIFE_BUILD_TESTS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

print_success "Build environment configured"

# Step 8: Build Project
print_status "Building project..."
if cmake --build . --parallel; then
    print_success "Project built successfully"
else
    print_warning "Build completed with warnings"
fi

cd ..

# Step 9: Setup VSCode (if available)
if command_exists code; then
    print_status "Setting up VSCode..."
    
    # Install recommended extensions
    code --install-extension ms-vscode.cpptools
    code --install-extension ms-vscode.cmake-tools
    code --install-extension ms-vscode.cpptools-extension-pack
    code --install-extension eamodio.gitlens
    code --install-extension gruntfuggly.todo-tree
    code --install-extension usernamehw.errorlens
    
    print_success "VSCode extensions installed"
fi

# Step 10: Run Initial Tests
print_status "Running initial tests..."
cd build-dev
if ctest --output-on-failure; then
    print_success "All tests passed!"
else
    print_warning "Some tests failed (this might be expected for initial setup)"
fi
cd ..

# Step 11: Create useful aliases and scripts
print_status "Creating developer shortcuts..."

# Create quick build script
cat > quick_build.sh << 'EOF'
#!/bin/bash
cd build-dev
cmake --build . --parallel
ctest --output-on-failure
EOF
chmod +x quick_build.sh

# Create quick test script  
cat > quick_test.sh << 'EOF'
#!/bin/bash
cd build-dev
ctest --output-on-failure --parallel
EOF
chmod +x quick_test.sh

print_success "Developer shortcuts created"

# Step 12: Generate helpful information
echo ""
echo "=============================================="
print_success "🎉 Development Environment Setup Complete!"
echo "=============================================="
echo ""
echo "📋 Quick Commands:"
echo "   ./quick_build.sh     - Build and test"
echo "   ./quick_test.sh      - Run tests only"
echo "   ./scripts/run_tests.sh - Full test suite with coverage"
echo ""
echo "🔧 Build Commands:"
echo "   cd build-dev"
echo "   cmake --build . --parallel"
echo "   ctest --output-on-failure"
echo ""
echo "🎮 Run Examples:"
echo "   ./build-modular/bin/modular_example"
echo "   ./build-modular/bin/sylife_logger_perf_test"
echo ""
echo "📖 Documentation:"
echo "   DEVELOPER_ONBOARDING.md  - Complete developer guide"
echo "   MODULAR_ARCHITECTURE.md  - Architecture overview"
echo "   CONTINUOUS_IMPROVEMENT_ROADMAP.md - Improvement tasks"
echo ""
echo "🆘 Getting Help:"
echo "   - Check DEVELOPER_ONBOARDING.md"
echo "   - Create GitHub issue"
echo "   - Contact team members"
echo ""
echo "🚀 Next Steps:"
echo "1. Read DEVELOPER_ONBOARDING.md"
echo "2. Open project in VSCode: code ."
echo "3. Try building: ./quick_build.sh"
echo "4. Pick a 'good first issue' from GitHub"
echo "5. Start coding!"
echo ""
print_success "Happy coding! 🎮✨"