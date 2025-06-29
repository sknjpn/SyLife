#!/bin/bash

# ECS System Generator
# Automatically generates system class with proper structure and tests

set -e

SYSTEM_NAME=""
MODULE_NAME=""
COMPONENTS=""
PRIORITY=0

usage() {
    echo "Usage: $0 -n <system_name> -m <module_name> [-c <components>] [-p <priority>]"
    echo ""
    echo "Options:"
    echo "  -n <name>       System name (e.g., Movement, Collision, Rendering)"
    echo "  -m <module>     Module name (e.g., ecs, physics, graphics)"
    echo "  -c <components> Required components in format 'Component1,Component2'"
    echo "                  Example: 'Transform,Velocity'"
    echo "  -p <priority>   System execution priority (default: 0, lower = earlier)"
    echo ""
    echo "Examples:"
    echo "  $0 -n Movement -m physics -c 'Transform,Velocity' -p 10"
    echo "  $0 -n Rendering -m graphics -c 'Transform,Sprite' -p 100"
    exit 1
}

# Parse command line arguments
while getopts "n:m:c:p:h" opt; do
    case $opt in
        n) SYSTEM_NAME="$OPTARG" ;;
        m) MODULE_NAME="$OPTARG" ;;
        c) COMPONENTS="$OPTARG" ;;
        p) PRIORITY="$OPTARG" ;;
        h) usage ;;
        *) usage ;;
    esac
done

# Validate required arguments
if [[ -z "$SYSTEM_NAME" ]] || [[ -z "$MODULE_NAME" ]]; then
    echo "❌ Error: System name and module name are required"
    usage
fi

# Normalize names
SYSTEM_NAME_LOWER=$(echo "$SYSTEM_NAME" | tr '[:upper:]' '[:lower:]')
SYSTEM_NAME_UPPER=$(echo "$SYSTEM_NAME" | tr '[:lower:]' '[:upper:]')
MODULE_DIR="modules/$MODULE_NAME"

# Create module directory structure if it doesn't exist
mkdir -p "$MODULE_DIR"/{include/sylife/$MODULE_NAME,src,tests}

# File paths
HEADER_FILE="$MODULE_DIR/include/sylife/$MODULE_NAME/${SYSTEM_NAME_LOWER}_system.h"
SOURCE_FILE="$MODULE_DIR/src/${SYSTEM_NAME_LOWER}_system.cpp"
TEST_FILE="$MODULE_DIR/tests/test_${SYSTEM_NAME_LOWER}_system.cpp"

echo "🔧 Generating system: $SYSTEM_NAME"
echo "📁 Module: $MODULE_NAME"
echo "📄 Header: $HEADER_FILE"
echo "📄 Source: $SOURCE_FILE"
echo "🧪 Test: $TEST_FILE"

# Generate component includes and template parameters
COMPONENT_INCLUDES=""
COMPONENT_TYPES=""
COMPONENT_QUERIES=""

if [[ -n "$COMPONENTS" ]]; then
    IFS=',' read -ra COMPONENT_ARRAY <<< "$COMPONENTS"
    for component in "${COMPONENT_ARRAY[@]}"; do
        component_lower=$(echo "$component" | tr '[:upper:]' '[:lower:]')
        COMPONENT_INCLUDES+="\n#include \"sylife/$MODULE_NAME/${component_lower}_component.h\""
        
        if [[ -z "$COMPONENT_TYPES" ]]; then
            COMPONENT_TYPES="$component"
        else
            COMPONENT_TYPES+=", $component"
        fi
        
        COMPONENT_QUERIES+="\n        auto& ${component_lower} = world.getComponent<${component}Component>(entity);"
    done
fi

# Generate header file
cat > "$HEADER_FILE" << EOF
#pragma once

#include "sylife/ecs/system.h"
#include "sylife/ecs/world.h"$COMPONENT_INCLUDES

namespace sylife::$MODULE_NAME {

/**
 * @brief ${SYSTEM_NAME} system for the ECS architecture
 * 
 * This system processes entities with the required components and
 * implements ${SYSTEM_NAME_LOWER} logic for the simulation.
 */
class ${SYSTEM_NAME}System : public ecs::System<${COMPONENT_TYPES}Component> {
public:
    /**
     * @brief Constructor
     */
    explicit ${SYSTEM_NAME}System();
    
    /**
     * @brief Destructor
     */
    ~${SYSTEM_NAME}System() override = default;
    
    /**
     * @brief Initialize the system
     * 
     * Called once when the system is registered with the world.
     */
    void init() override;
    
    /**
     * @brief Update the system
     * 
     * Called every frame to process entities.
     * 
     * @param deltaTime Time elapsed since last update in seconds
     */
    void update(float deltaTime) override;
    
    /**
     * @brief Shutdown the system
     * 
     * Called when the system is being destroyed or removed.
     */
    void shutdown() override;
    
    /**
     * @brief Get system execution priority
     * 
     * Lower numbers execute earlier in the frame.
     * 
     * @return System priority
     */
    int getPriority() const override { return $PRIORITY; }
    
    /**
     * @brief Get system name for debugging
     * 
     * @return Human-readable system name
     */
    const char* getName() const override { return "${SYSTEM_NAME}System"; }

private:
    /**
     * @brief Process a single entity
     * 
     * @param entity The entity to process
     * @param deltaTime Time elapsed since last update
     */
    void processEntity(ecs::Entity entity, float deltaTime);
    
    /**
     * @brief Validate entity has required components
     * 
     * @param entity The entity to validate
     * @return true if entity has all required components
     */
    bool validateEntity(ecs::Entity entity) const;
    
    // System state
    bool m_initialized = false;
    size_t m_processedEntities = 0;
    float m_totalTime = 0.0f;
};

} // namespace sylife::$MODULE_NAME
EOF

# Generate source file
cat > "$SOURCE_FILE" << EOF
#include "sylife/$MODULE_NAME/${SYSTEM_NAME_LOWER}_system.h"
#include "sylife/core/logging.h"
#include <algorithm>
#include <chrono>

namespace sylife::$MODULE_NAME {

${SYSTEM_NAME}System::${SYSTEM_NAME}System() {
    SYLIFE_LOG_DEBUG("${SYSTEM_NAME}System created");
}

void ${SYSTEM_NAME}System::init() {
    if (m_initialized) {
        SYLIFE_LOG_WARNING("${SYSTEM_NAME}System already initialized");
        return;
    }
    
    SYLIFE_LOG_INFO("Initializing ${SYSTEM_NAME}System");
    
    // Initialize system-specific resources
    m_processedEntities = 0;
    m_totalTime = 0.0f;
    
    m_initialized = true;
    SYLIFE_LOG_INFO("${SYSTEM_NAME}System initialized successfully");
}

void ${SYSTEM_NAME}System::update(float deltaTime) {
    if (!m_initialized) {
        SYLIFE_LOG_ERROR("${SYSTEM_NAME}System not initialized");
        return;
    }
    
    if (!isEnabled()) {
        return;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Get world reference
    const auto& world = getWorld();
    
    // Query entities with required components
    const auto entities = world.getEntitiesWith<${COMPONENT_TYPES}Component>();
    
    // Process each entity
    for (const auto entity : entities) {
        if (validateEntity(entity)) {
            processEntity(entity, deltaTime);
            ++m_processedEntities;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<float>(end - start).count();
    m_totalTime += duration;
    
    // Performance logging (only log occasionally to avoid spam)
    static size_t logCounter = 0;
    if (++logCounter % 1000 == 0) {
        SYLIFE_LOG_DEBUG("${SYSTEM_NAME}System processed {} entities in {:.3f}ms", 
                        entities.size(), duration * 1000.0f);
    }
}

void ${SYSTEM_NAME}System::shutdown() {
    if (!m_initialized) {
        return;
    }
    
    SYLIFE_LOG_INFO("Shutting down ${SYSTEM_NAME}System");
    SYLIFE_LOG_INFO("Total entities processed: {}", m_processedEntities);
    SYLIFE_LOG_INFO("Total execution time: {:.3f}s", m_totalTime);
    
    // Cleanup system-specific resources
    
    m_initialized = false;
}

void ${SYSTEM_NAME}System::processEntity(ecs::Entity entity, float deltaTime) {
    // Get world reference
    auto& world = getWorld();
    
    // Get required components$COMPONENT_QUERIES
    
    // TODO: Implement ${SYSTEM_NAME_LOWER} logic here
    // Example:
    // transform.x += velocity.x * deltaTime;
    // transform.y += velocity.y * deltaTime;
    
    (void)deltaTime; // Suppress unused parameter warning
}

bool ${SYSTEM_NAME}System::validateEntity(ecs::Entity entity) const {
    const auto& world = getWorld();
    
    // Check if entity is valid
    if (!world.isEntityValid(entity)) {
        return false;
    }
    
    // Check if entity has all required components
    return world.hasComponents<${COMPONENT_TYPES}Component>(entity);
}

} // namespace sylife::$MODULE_NAME
EOF

# Generate test file
cat > "$TEST_FILE" << EOF
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "sylife/$MODULE_NAME/${SYSTEM_NAME_LOWER}_system.h"
#include "sylife/ecs/world.h"$COMPONENT_INCLUDES

using namespace sylife::$MODULE_NAME;
using namespace sylife::ecs;

class ${SYSTEM_NAME}SystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        world = std::make_unique<World>();
        system = std::make_unique<${SYSTEM_NAME}System>();
        
        // Register system with world
        world->registerSystem(system.get());
        system->init();
    }
    
    void TearDown() override {
        if (system) {
            system->shutdown();
        }
        system.reset();
        world.reset();
    }
    
    Entity createTestEntity() {
        auto entity = world->createEntity();
        
        // Add required components
        // TODO: Add component creation based on actual requirements
        /*
        world->addComponent<TransformComponent>(entity, TransformComponent{});
        world->addComponent<VelocityComponent>(entity, VelocityComponent{});
        */
        
        return entity;
    }
    
    std::unique_ptr<World> world;
    std::unique_ptr<${SYSTEM_NAME}System> system;
};

TEST_F(${SYSTEM_NAME}SystemTest, Construction) {
    // System should be constructed successfully
    EXPECT_NE(system, nullptr);
    EXPECT_STREQ(system->getName(), "${SYSTEM_NAME}System");
    EXPECT_EQ(system->getPriority(), $PRIORITY);
}

TEST_F(${SYSTEM_NAME}SystemTest, Initialization) {
    // System should initialize without errors
    // (Already initialized in SetUp)
    EXPECT_TRUE(system->isEnabled());
}

TEST_F(${SYSTEM_NAME}SystemTest, UpdateWithoutEntities) {
    // Should handle update with no entities gracefully
    system->update(0.016f); // 60 FPS delta time
    
    // No entities to process, should complete without error
    EXPECT_TRUE(system->isEnabled());
}

TEST_F(${SYSTEM_NAME}SystemTest, UpdateWithValidEntity) {
    // Create entity with required components
    auto entity = createTestEntity();
    
    // Update system
    system->update(0.016f);
    
    // Entity should still be valid
    EXPECT_TRUE(world->isEntityValid(entity));
    
    // TODO: Add specific behavior tests based on system logic
}

TEST_F(${SYSTEM_NAME}SystemTest, UpdateWithInvalidEntity) {
    // Create entity but don't add required components
    auto entity = world->createEntity();
    
    // Update system
    system->update(0.016f);
    
    // Should handle invalid entity gracefully
    EXPECT_TRUE(system->isEnabled());
}

TEST_F(${SYSTEM_NAME}SystemTest, EnableDisable) {
    // System should start enabled
    EXPECT_TRUE(system->isEnabled());
    
    // Disable system
    system->setEnabled(false);
    EXPECT_FALSE(system->isEnabled());
    
    // Create entity and update
    auto entity = createTestEntity();
    system->update(0.016f);
    
    // System should not process entities when disabled
    // TODO: Add specific checks based on system behavior
    
    // Re-enable system
    system->setEnabled(true);
    EXPECT_TRUE(system->isEnabled());
}

TEST_F(${SYSTEM_NAME}SystemTest, MultipleEntities) {
    constexpr size_t entityCount = 100;
    std::vector<Entity> entities;
    
    // Create multiple entities
    for (size_t i = 0; i < entityCount; ++i) {
        entities.push_back(createTestEntity());
    }
    
    // Update system
    system->update(0.016f);
    
    // All entities should still be valid
    for (const auto entity : entities) {
        EXPECT_TRUE(world->isEntityValid(entity));
    }
}

TEST_F(${SYSTEM_NAME}SystemTest, PerformanceUpdate) {
    constexpr size_t entityCount = 1000;
    std::vector<Entity> entities;
    
    // Create many entities
    for (size_t i = 0; i < entityCount; ++i) {
        entities.push_back(createTestEntity());
    }
    
    // Measure update performance
    auto start = std::chrono::high_resolution_clock::now();
    
    system->update(0.016f);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Should process 1000 entities in reasonable time (< 10ms)
    EXPECT_LT(duration.count(), 10000);
    
    std::cout << "Processed " << entityCount << " entities in " 
              << duration.count() << " microseconds" << std::endl;
}

TEST_F(${SYSTEM_NAME}SystemTest, Shutdown) {
    // System should shutdown cleanly
    system->shutdown();
    
    // Should handle multiple shutdowns gracefully
    system->shutdown();
}

// Integration tests
TEST_F(${SYSTEM_NAME}SystemTest, IntegrationWithWorld) {
    // Test system integration with world
    auto entity = createTestEntity();
    
    // Update world (which should update the system)
    world->update(0.016f);
    
    // Entity should still be valid
    EXPECT_TRUE(world->isEntityValid(entity));
}

// Stress tests
TEST_F(${SYSTEM_NAME}SystemTest, StressTestManyUpdates) {
    auto entity = createTestEntity();
    
    // Perform many updates
    for (int i = 0; i < 10000; ++i) {
        system->update(0.001f); // 1ms delta time
    }
    
    // Entity should still be valid
    EXPECT_TRUE(world->isEntityValid(entity));
}
EOF

# Make the script executable
chmod +x "$0"

echo "✅ System generation complete!"
echo ""
echo "📋 Next steps:"
echo "1. Review and customize the generated files"
echo "2. Implement the processEntity() method with actual logic"
echo "3. Add proper component includes and requirements"
echo "4. Run tests: ctest --test-dir build -R ${SYSTEM_NAME}System"
echo "5. Add to version control: git add $HEADER_FILE $SOURCE_FILE $TEST_FILE"
echo ""
echo "💡 Usage example:"
echo "   #include \"sylife/$MODULE_NAME/${SYSTEM_NAME_LOWER}_system.h\""
echo "   auto system = std::make_unique<${SYSTEM_NAME}System>();"
echo "   world.registerSystem(system.get());"
EOF