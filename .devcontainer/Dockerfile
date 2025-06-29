# SyLife Development Container
# Based on official Microsoft C++ development container with additional tools

FROM mcr.microsoft.com/devcontainers/cpp:0-ubuntu-22.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive
ENV SYLIFE_DEV_CONTAINER=1

# Install system dependencies
RUN apt-get update && apt-get install -y \
    # Build essentials
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    \
    # Development tools
    git \
    curl \
    wget \
    unzip \
    \
    # C++ development tools
    clang-12 \
    clang-format \
    clang-tidy \
    cppcheck \
    \
    # Testing and analysis
    libgtest-dev \
    libgmock-dev \
    valgrind \
    gcovr \
    lcov \
    \
    # OpenGL/Graphics libraries (for potential future GUI needs)
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    xorg-dev \
    \
    # Audio libraries (for Siv3D compatibility)
    libasound2-dev \
    \
    # Python tools
    python3 \
    python3-pip \
    \
    # Documentation tools
    doxygen \
    graphviz \
    \
    # Performance tools
    htop \
    iotop \
    strace \
    lsof \
    \
    # Editor and utilities
    vim \
    nano \
    tree \
    jq \
    && rm -rf /var/lib/apt/lists/*

# Install Google Test properly
RUN cd /usr/src/gtest && \
    cmake . && \
    cmake --build . && \
    cp lib/*.a /usr/lib/ && \
    cd /usr/src/gmock && \
    cmake . && \
    cmake --build . && \
    cp lib/*.a /usr/lib/ || true

# Install Python development tools
RUN pip3 install --no-cache-dir \
    pre-commit \
    cpplint \
    black \
    flake8 \
    mypy \
    pytest \
    coverage

# Install Node.js tools for web development
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs && \
    npm install -g \
    http-server \
    live-server

# Set up non-root user for development
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Configure git for the user
RUN git config --system --add safe.directory '*' && \
    git config --system pull.rebase true && \
    git config --system init.defaultBranch main && \
    git config --system core.autocrlf input

# Install modern CMake (latest version)
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | gpg --dearmor - > /usr/share/keyrings/kitware-archive-keyring.gpg && \
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' > /etc/apt/sources.list.d/kitware.list && \
    apt-get update && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

# Install ccache for faster builds
RUN apt-get update && apt-get install -y ccache && \
    rm -rf /var/lib/apt/lists/* && \
    ccache --set-config=max_size=2G && \
    ccache --set-config=compression=true

# Create useful aliases
RUN echo 'alias ll="ls -alF"' >> /etc/bash.bashrc && \
    echo 'alias la="ls -A"' >> /etc/bash.bashrc && \
    echo 'alias l="ls -CF"' >> /etc/bash.bashrc && \
    echo 'alias build="cmake --build build --parallel"' >> /etc/bash.bashrc && \
    echo 'alias test="cd build && ctest --output-on-failure && cd .."' >> /etc/bash.bashrc && \
    echo 'alias clean="rm -rf build*"' >> /etc/bash.bashrc

# Set up workspace
WORKDIR /workspaces/SyLife

# Set default shell
SHELL ["/bin/bash", "-c"]

# Create convenience script for setup
RUN echo '#!/bin/bash' > /usr/local/bin/sylife-setup && \
    echo 'echo "🚀 Setting up SyLife development environment..."' >> /usr/local/bin/sylife-setup && \
    echo 'cd /workspaces/SyLife' >> /usr/local/bin/sylife-setup && \
    echo 'git submodule update --init --recursive' >> /usr/local/bin/sylife-setup && \
    echo 'mkdir -p build-container' >> /usr/local/bin/sylife-setup && \
    echo 'cd build-container' >> /usr/local/bin/sylife-setup && \
    echo 'cmake .. -DCMAKE_BUILD_TYPE=Debug -DSYLIFE_BUILD_TESTS=ON -GNinja' >> /usr/local/bin/sylife-setup && \
    echo 'cmake --build . --parallel' >> /usr/local/bin/sylife-setup && \
    echo 'echo "✅ SyLife development environment ready!"' >> /usr/local/bin/sylife-setup && \
    chmod +x /usr/local/bin/sylife-setup

# Welcome message
RUN echo 'echo "🎮 Welcome to SyLife Development Container!"' >> /etc/bash.bashrc && \
    echo 'echo "Quick commands:"' >> /etc/bash.bashrc && \
    echo 'echo "  sylife-setup  - Initialize development environment"' >> /etc/bash.bashrc && \
    echo 'echo "  build         - Build the project"' >> /etc/bash.bashrc && \
    echo 'echo "  test          - Run tests"' >> /etc/bash.bashrc && \
    echo 'echo "  clean         - Clean build files"' >> /etc/bash.bashrc && \
    echo 'echo ""' >> /etc/bash.bashrc