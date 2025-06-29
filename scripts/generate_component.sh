#!/bin/bash

# ECS Component Generator
# Automatically generates component class with proper structure and tests

set -e

COMPONENT_NAME=""
MODULE_NAME=""
FIELDS=""

usage() {
    echo "Usage: $0 -n <component_name> -m <module_name> [-f <fields>]"
    echo ""
    echo "Options:"
    echo "  -n <name>     Component name (e.g., Transform, Health, Velocity)"
    echo "  -m <module>   Module name (e.g., ecs, physics, gameplay)"
    echo "  -f <fields>   Component fields in format 'type:name,type:name'"
    echo "                Example: 'float:x,float:y,float:z'"
    echo ""
    echo "Examples:"
    echo "  $0 -n Transform -m ecs -f 'float:x,float:y,float:rotation'"
    echo "  $0 -n Health -m gameplay -f 'float:current,float:maximum'"
    exit 1
}

# Parse command line arguments
while getopts "n:m:f:h" opt; do
    case $opt in
        n) COMPONENT_NAME="$OPTARG" ;;
        m) MODULE_NAME="$OPTARG" ;;
        f) FIELDS="$OPTARG" ;;
        h) usage ;;
        *) usage ;;
    esac
done

# Validate required arguments
if [[ -z "$COMPONENT_NAME" ]] || [[ -z "$MODULE_NAME" ]]; then
    echo "❌ Error: Component name and module name are required"
    usage
fi

# Normalize names
COMPONENT_NAME_LOWER=$(echo "$COMPONENT_NAME" | tr '[:upper:]' '[:lower:]')
COMPONENT_NAME_UPPER=$(echo "$COMPONENT_NAME" | tr '[:lower:]' '[:upper:]')
MODULE_DIR="modules/$MODULE_NAME"

# Create module directory structure if it doesn't exist
mkdir -p "$MODULE_DIR"/{include/sylife/$MODULE_NAME,src,tests}

# File paths
HEADER_FILE="$MODULE_DIR/include/sylife/$MODULE_NAME/${COMPONENT_NAME_LOWER}_component.h"
TEST_FILE="$MODULE_DIR/tests/test_${COMPONENT_NAME_LOWER}_component.cpp"

echo "🔧 Generating component: $COMPONENT_NAME"
echo "📁 Module: $MODULE_NAME"
echo "📄 Header: $HEADER_FILE"
echo "🧪 Test: $TEST_FILE"

# Generate component fields
FIELD_DECLARATIONS=""
FIELD_INITIALIZERS=""
FIELD_GETTERS_SETTERS=""
FIELD_COMPARISONS=""
FIELD_SERIALIZATIONS=""

if [[ -n "$FIELDS" ]]; then
    IFS=',' read -ra FIELD_ARRAY <<< "$FIELDS"
    for field in "${FIELD_ARRAY[@]}"; do
        if [[ "$field" =~ ^([^:]+):([^:]+)$ ]]; then
            field_type="${BASH_REMATCH[1]}"
            field_name="${BASH_REMATCH[2]}"
            
            # Field declaration
            FIELD_DECLARATIONS+="\n    ${field_type} ${field_name};"
            
            # Constructor initializer
            if [[ -z "$FIELD_INITIALIZERS" ]]; then
                FIELD_INITIALIZERS="${field_name}(${field_type}{})"
            else
                FIELD_INITIALIZERS+=", ${field_name}(${field_type}{})"
            fi
            
            # Getters and setters
            FIELD_GETTERS_SETTERS+="\n    [[nodiscard]] ${field_type} get${field_name^}() const noexcept { return ${field_name}; }"
            FIELD_GETTERS_SETTERS+="\n    void set${field_name^}(${field_type} value) noexcept { ${field_name} = value; }"
            
            # Equality comparison
            if [[ -z "$FIELD_COMPARISONS" ]]; then
                FIELD_COMPARISONS="${field_name} == other.${field_name}"
            else
                FIELD_COMPARISONS+=" && ${field_name} == other.${field_name}"
            fi
            
            # Serialization
            FIELD_SERIALIZATIONS+="\n        ar & ${field_name};"
        fi
    done
fi

# Generate header file
cat > "$HEADER_FILE" << EOF
#pragma once

#include <type_traits>
#include <memory>

namespace sylife::$MODULE_NAME {

/**
 * @brief ${COMPONENT_NAME} component for the ECS system
 * 
 * This component provides ${COMPONENT_NAME_LOWER} functionality for entities.
 * It follows the data-oriented design principles of the ECS architecture.
 */
class ${COMPONENT_NAME}Component {
public:
    // Type traits for ECS system
    static constexpr bool is_copyable = true;
    static constexpr bool is_serializable = true;
    
    /**
     * @brief Default constructor
     */
    ${COMPONENT_NAME}Component() noexcept : $FIELD_INITIALIZERS {}
    
    /**
     * @brief Parameterized constructor
     */
    explicit ${COMPONENT_NAME}Component($(echo "$FIELDS" | sed 's/,/, /g' | sed 's/:/: /g')) noexcept 
        : $FIELD_INITIALIZERS {}
    
    /**
     * @brief Copy constructor
     */
    ${COMPONENT_NAME}Component(const ${COMPONENT_NAME}Component&) = default;
    
    /**
     * @brief Move constructor
     */
    ${COMPONENT_NAME}Component(${COMPONENT_NAME}Component&&) noexcept = default;
    
    /**
     * @brief Copy assignment operator
     */
    ${COMPONENT_NAME}Component& operator=(const ${COMPONENT_NAME}Component&) = default;
    
    /**
     * @brief Move assignment operator
     */
    ${COMPONENT_NAME}Component& operator=(${COMPONENT_NAME}Component&&) noexcept = default;
    
    /**
     * @brief Destructor
     */
    ~${COMPONENT_NAME}Component() = default;
    
    // Accessors$FIELD_GETTERS_SETTERS
    
    /**
     * @brief Equality comparison operator
     */
    bool operator==(const ${COMPONENT_NAME}Component& other) const noexcept {
        return $FIELD_COMPARISONS;
    }
    
    /**
     * @brief Inequality comparison operator
     */
    bool operator!=(const ${COMPONENT_NAME}Component& other) const noexcept {
        return !(*this == other);
    }
    
    /**
     * @brief Reset component to default state
     */
    void reset() noexcept {
        *this = ${COMPONENT_NAME}Component{};
    }
    
    /**
     * @brief Check if component is in default state
     */
    [[nodiscard]] bool isDefault() const noexcept {
        return *this == ${COMPONENT_NAME}Component{};
    }
    
    /**
     * @brief Serialization support
     */
    template<class Archive>
    void serialize(Archive& ar) {$FIELD_SERIALIZATIONS
    }

private:
    // Component data$FIELD_DECLARATIONS
};

// Type traits specializations
static_assert(std::is_trivially_copyable_v<${COMPONENT_NAME}Component>, 
              "${COMPONENT_NAME}Component should be trivially copyable for performance");
static_assert(std::is_standard_layout_v<${COMPONENT_NAME}Component>, 
              "${COMPONENT_NAME}Component should have standard layout");

} // namespace sylife::$MODULE_NAME

/**
 * @brief Hash function for ${COMPONENT_NAME}Component
 */
template<>
struct std::hash<sylife::${MODULE_NAME}::${COMPONENT_NAME}Component> {
    size_t operator()(const sylife::${MODULE_NAME}::${COMPONENT_NAME}Component& component) const noexcept {
        // Simple hash combination - can be improved based on actual fields
        size_t seed = 0;
        // TODO: Add proper hash calculation based on component fields
        return seed;
    }
};
EOF

# Generate test file
cat > "$TEST_FILE" << EOF
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sylife/$MODULE_NAME/${COMPONENT_NAME_LOWER}_component.h"

using namespace sylife::$MODULE_NAME;

class ${COMPONENT_NAME}ComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test fixtures
    }
    
    void TearDown() override {
        // Cleanup test fixtures
    }
};

TEST_F(${COMPONENT_NAME}ComponentTest, DefaultConstruction) {
    ${COMPONENT_NAME}Component component;
    
    // Verify default state
    EXPECT_TRUE(component.isDefault());
}

TEST_F(${COMPONENT_NAME}ComponentTest, ParameterizedConstruction) {
    // TODO: Add parameterized construction tests based on actual fields
    ${COMPONENT_NAME}Component component;
    
    // Verify initialization
    EXPECT_TRUE(component.isDefault());
}

TEST_F(${COMPONENT_NAME}ComponentTest, CopyConstruction) {
    ${COMPONENT_NAME}Component original;
    ${COMPONENT_NAME}Component copy(original);
    
    EXPECT_EQ(original, copy);
}

TEST_F(${COMPONENT_NAME}ComponentTest, MoveConstruction) {
    ${COMPONENT_NAME}Component original;
    ${COMPONENT_NAME}Component originalCopy = original;
    ${COMPONENT_NAME}Component moved(std::move(original));
    
    EXPECT_EQ(originalCopy, moved);
}

TEST_F(${COMPONENT_NAME}ComponentTest, CopyAssignment) {
    ${COMPONENT_NAME}Component original;
    ${COMPONENT_NAME}Component assigned;
    
    assigned = original;
    
    EXPECT_EQ(original, assigned);
}

TEST_F(${COMPONENT_NAME}ComponentTest, MoveAssignment) {
    ${COMPONENT_NAME}Component original;
    ${COMPONENT_NAME}Component originalCopy = original;
    ${COMPONENT_NAME}Component assigned;
    
    assigned = std::move(original);
    
    EXPECT_EQ(originalCopy, assigned);
}

TEST_F(${COMPONENT_NAME}ComponentTest, EqualityComparison) {
    ${COMPONENT_NAME}Component component1;
    ${COMPONENT_NAME}Component component2;
    
    EXPECT_EQ(component1, component2);
    EXPECT_FALSE(component1 != component2);
}

TEST_F(${COMPONENT_NAME}ComponentTest, Reset) {
    ${COMPONENT_NAME}Component component;
    
    // Modify component state
    // TODO: Add field modifications based on actual fields
    
    component.reset();
    
    EXPECT_TRUE(component.isDefault());
}

// Performance tests
TEST_F(${COMPONENT_NAME}ComponentTest, PerformanceCreateDestroy) {
    constexpr size_t iterations = 100000;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        ${COMPONENT_NAME}Component component;
        // Prevent optimization
        volatile auto* ptr = &component;
        (void)ptr;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should be very fast for simple components
    EXPECT_LT(duration.count(), 100000); // Less than 100ms for 100k iterations
}

TEST_F(${COMPONENT_NAME}ComponentTest, PerformanceCopy) {
    constexpr size_t iterations = 100000;
    ${COMPONENT_NAME}Component source;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        ${COMPONENT_NAME}Component copy = source;
        // Prevent optimization
        volatile auto* ptr = &copy;
        (void)ptr;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should be very fast for copyable components
    EXPECT_LT(duration.count(), 100000); // Less than 100ms for 100k iterations
}

// Serialization tests (if serialization support is needed)
/*
TEST_F(${COMPONENT_NAME}ComponentTest, Serialization) {
    ${COMPONENT_NAME}Component original;
    
    // TODO: Add serialization tests when serialization library is available
    
    // std::stringstream ss;
    // boost::archive::text_oarchive oa(ss);
    // oa << original;
    // 
    // ${COMPONENT_NAME}Component deserialized;
    // boost::archive::text_iarchive ia(ss);
    // ia >> deserialized;
    // 
    // EXPECT_EQ(original, deserialized);
}
*/
EOF

# Update CMakeLists.txt if it exists
if [[ -f "$MODULE_DIR/CMakeLists.txt" ]]; then
    echo "🔧 Updating CMakeLists.txt..."
    # Add to CMakeLists.txt if not already present
    if ! grep -q "${COMPONENT_NAME_LOWER}_component.h" "$MODULE_DIR/CMakeLists.txt"; then
        sed -i "/set.*HEADERS/a\\    include/sylife/$MODULE_NAME/${COMPONENT_NAME_LOWER}_component.h" "$MODULE_DIR/CMakeLists.txt"
    fi
fi

echo "✅ Component generation complete!"
echo ""
echo "📋 Next steps:"
echo "1. Review and customize the generated files"
echo "2. Add proper field implementations if needed"
echo "3. Update hash function implementation"
echo "4. Run tests: ctest --test-dir build -R ${COMPONENT_NAME}Component"
echo "5. Add to version control: git add $HEADER_FILE $TEST_FILE"
echo ""
echo "💡 Usage example:"
echo "   #include \"sylife/$MODULE_NAME/${COMPONENT_NAME_LOWER}_component.h\""
echo "   auto entity = world.createEntity();"
echo "   world.addComponent<${COMPONENT_NAME}Component>(entity);"
EOF