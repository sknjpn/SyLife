#!/bin/bash

# Pre-commit Hook Setup Script
# Installs and configures pre-commit hooks for the SyLife project

set -e

echo "🔧 Setting up pre-commit hooks for SyLife..."

# Check if pre-commit is installed
if ! command -v pre-commit &> /dev/null; then
    echo "📦 Installing pre-commit..."
    if command -v pip &> /dev/null; then
        pip install pre-commit
    elif command -v pip3 &> /dev/null; then
        pip3 install pre-commit
    else
        echo "❌ pip not found. Please install pre-commit manually:"
        echo "   pip install pre-commit"
        exit 1
    fi
fi

# Install pre-commit hooks
echo "⚙️ Installing pre-commit hooks..."
pre-commit install

# Install commit-msg hook for conventional commits
echo "📝 Installing commit message hook..."
pre-commit install --hook-type commit-msg

# Generate secrets baseline
echo "🔒 Generating secrets baseline..."
if command -v detect-secrets &> /dev/null; then
    detect-secrets scan --baseline .secrets.baseline
else
    echo "⚠️ detect-secrets not installed, skipping secrets baseline"
fi

# Test the hooks
echo "🧪 Testing pre-commit hooks..."
pre-commit run --all-files || {
    echo "⚠️ Some pre-commit checks failed, but that's normal for first setup"
    echo "   The hooks will catch issues on future commits"
}

echo "✅ Pre-commit hooks setup complete!"
echo ""
echo "💡 Usage:"
echo "   - Hooks run automatically on each commit"
echo "   - To run manually: pre-commit run --all-files"
echo "   - To skip hooks (emergency): git commit --no-verify"