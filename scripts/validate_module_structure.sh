#!/bin/bash

# Module Structure Validator
# Ensures that modules follow the expected directory structure

set -e

VIOLATIONS=()

for file in "$@"; do
    # Check if file is in a module directory
    if [[ ! "$file" =~ ^modules/ ]]; then
        continue
    fi
    
    # Extract module name
    MODULE=$(echo "$file" | cut -d'/' -f2)
    MODULE_DIR="modules/$MODULE"
    
    # Skip if not a real module directory
    if [[ ! -d "$MODULE_DIR" ]]; then
        continue
    fi
    
    # Required directories
    REQUIRED_DIRS=("include" "src" "tests")
    
    for dir in "${REQUIRED_DIRS[@]}"; do
        if [[ ! -d "$MODULE_DIR/$dir" ]]; then
            VIOLATIONS+=("Missing directory: $MODULE_DIR/$dir")
        fi
    done
    
    # Check for CMakeLists.txt
    if [[ ! -f "$MODULE_DIR/CMakeLists.txt" ]]; then
        VIOLATIONS+=("Missing CMakeLists.txt in $MODULE_DIR")
    fi
    
    # Check include structure
    if [[ -d "$MODULE_DIR/include" ]] && [[ ! -d "$MODULE_DIR/include/sylife" ]]; then
        VIOLATIONS+=("Include directory should contain sylife subdirectory: $MODULE_DIR/include/sylife")
    fi
    
    # Check that header files are in include directory
    if [[ "$file" =~ \.hpp?$ ]] && [[ ! "$file" =~ ^modules/.*/include/ ]]; then
        VIOLATIONS+=("Header file should be in include directory: $file")
    fi
    
    # Check that source files are in src directory
    if [[ "$file" =~ \.cpp$ ]] && [[ ! "$file" =~ ^modules/.*/src/ ]] && [[ ! "$file" =~ ^modules/.*/tests/ ]]; then
        VIOLATIONS+=("Source file should be in src or tests directory: $file")
    fi
done

if [[ ${#VIOLATIONS[@]} -gt 0 ]]; then
    echo "❌ Module structure violations:"
    printf '%s\n' "${VIOLATIONS[@]}"
    echo ""
    echo "💡 Expected module structure:"
    echo "modules/<module>/"
    echo "├── CMakeLists.txt"
    echo "├── include/sylife/<module>/"
    echo "├── src/"
    echo "└── tests/"
    exit 1
fi

echo "✅ Module structure is valid"