#pragma once

#include "common.h"
#include "object.h"
#include "../physics/spatial_partitioning.h"
#include <memory>
#include <vector>

namespace sylife::core {

// Forward declarations
class AssetManager;
class PhysicsWorld;
class StateManager;

/**
 * @brief Main world class that manages the entire simulation
 */
class World : public Object {
public:
    /**
     * @brief Constructor
     */
    World();

    /**
     * @brief Destructor
     */
    ~World() override;

    // Singleton pattern
    static World* getInstance();
    static void createInstance();
    static void destroyInstance();

    /**
     * @brief Initialize the world
     * @param width World width in tiles
     * @param height World height in tiles
     */
    void initialize(int width, int height);

    /**
     * @brief Update the world simulation
     * @param deltaTime Time step
     */
    void update(double deltaTime);

    /**
     * @brief Render the world
     */
    void render();

    /**
     * @brief Set world name
     * @param name World name
     */
    void setName(const s3d::String& name);

    /**
     * @brief Get world name
     * @return World name
     */
    const s3d::String& getName() const;

    /**
     * @brief Set tile size
     * @param size Tile grid size
     */
    void setTileSize(const s3d::Point& size);

    /**
     * @brief Get tile size
     * @return Tile grid size
     */
    const s3d::Point& getTileSize() const;

    /**
     * @brief Get asset manager
     * @return Reference to asset manager
     */
    AssetManager& getAssetManager();

    /**
     * @brief Get physics world
     * @return Reference to physics world
     */
    PhysicsWorld& getPhysicsWorld();

    /**
     * @brief Get state manager
     * @return Reference to state manager
     */
    StateManager& getStateManager();

    /**
     * @brief Save world to file
     * @param filePath File path to save to
     */
    void saveToFile(const s3d::FilePath& filePath);

    /**
     * @brief Load world from file
     * @param filePath File path to load from
     */
    void loadFromFile(const s3d::FilePath& filePath);

    // Object interface
    void save(s3d::JSON& json) const override;
    void load(const s3d::JSON& json) override;
    s3d::String getTypeName() const override;
    std::unique_ptr<Object> clone() const override;
    bool isValid() const override;

private:
    /**
     * @brief Update physics simulation
     * @param deltaTime Time step
     */
    void updatePhysics(double deltaTime);

    /**
     * @brief Update game logic
     * @param deltaTime Time step
     */
    void updateLogic(double deltaTime);

    /**
     * @brief Update spatial partitioning
     */
    void updateSpatialPartitioning();

    // Static instance for singleton
    static std::unique_ptr<World> instance_;

    // World properties
    s3d::String name_;
    s3d::Point tileSize_;
    s3d::Vec2 fieldSize_;
    bool isInitialized_;

    // Subsystems
    std::unique_ptr<AssetManager> assetManager_;
    std::unique_ptr<PhysicsWorld> physicsWorld_;
    std::unique_ptr<StateManager> stateManager_;

    // Noise generators for environmental effects
    s3d::PerlinNoise perlinNoiseX_;
    s3d::PerlinNoise perlinNoiseY_;
    double waveInterval_;
    double elementPerTile_;
    double waveVelocityMax_;

    // Performance metrics
    mutable double lastUpdateTime_;
    mutable double lastRenderTime_;
    mutable size_t frameCount_;
};

/**
 * @brief RAII guard for world operations
 */
class WorldGuard {
public:
    WorldGuard();
    ~WorldGuard();

    WorldGuard(const WorldGuard&) = delete;
    WorldGuard& operator=(const WorldGuard&) = delete;

    WorldGuard(WorldGuard&&) = delete;
    WorldGuard& operator=(WorldGuard&&) = delete;

    World* get() const;
    World* operator->() const;
    World& operator*() const;

private:
    World* world_;
};

} // namespace sylife::core