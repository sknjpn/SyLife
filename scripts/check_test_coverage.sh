#!/bin/bash

# Test Coverage Checker
# Ensures that new source files have corresponding test files

set -e

MISSING_TESTS=()

for file in "$@"; do
    # Skip if it's already a test file
    if [[ "$file" =~ test_.*\.(cpp|hpp)$ ]] || [[ "$file" =~ .*_test\.(cpp|hpp)$ ]]; then
        continue
    fi
    
    # Skip header files (assume they're tested through implementation)
    if [[ "$file" =~ \.hpp?$ ]]; then
        continue
    fi
    
    # Check if this is a source file in modules
    if [[ "$file" =~ ^modules/.*/src/.*\.cpp$ ]]; then
        # Extract module name and filename
        MODULE=$(echo "$file" | sed 's|^modules/\([^/]*\)/.*|\1|')
        FILENAME=$(basename "$file" .cpp)
        
        # Expected test file location
        TEST_FILE="modules/$MODULE/tests/test_${FILENAME}.cpp"
        
        if [[ ! -f "$TEST_FILE" ]]; then
            MISSING_TESTS+=("$file -> $TEST_FILE")
        fi
    fi
done

if [[ ${#MISSING_TESTS[@]} -gt 0 ]]; then
    echo "❌ Missing test files for:"
    printf '%s\n' "${MISSING_TESTS[@]}"
    echo ""
    echo "💡 Create test files using: scripts/generate_test.sh <source_file>"
    exit 1
fi

echo "✅ All source files have corresponding tests"