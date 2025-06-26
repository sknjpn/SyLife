#pragma once

#include "sylife/core/common.h"
#include "sylife/core/object.h"
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

namespace sylife::states {

// Forward declarations
class State;
class CellState;
class TileState;
class EggState;
class PartState;

/**
 * @brief Central manager for all game states
 */
class StateManager : public core::Object {
public:
    /**
     * @brief Constructor
     */
    StateManager();

    /**
     * @brief Destructor
     */
    ~StateManager() override;

    /**
     * @brief Initialize state manager with grid size
     * @param gridSize Grid dimensions
     */
    void initialize(const s3d::Point& gridSize);

    /**
     * @brief Update all states
     * @param deltaTime Time step
     */
    void update(double deltaTime);

    /**
     * @brief Create a new state of specified type
     * @tparam T State type (must inherit from State)
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Shared pointer to created state
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> createState(Args&&... args) {
        static_assert(std::is_base_of_v<State, T>, "T must inherit from State");
        
        auto state = std::make_shared<T>(std::forward<Args>(args)...);
        registerState(state);
        return state;
    }

    /**
     * @brief Register an existing state
     * @param state State to register
     */
    void registerState(std::shared_ptr<State> state);

    /**
     * @brief Unregister a state
     * @param state State to unregister
     */
    void unregisterState(std::shared_ptr<State> state);

    /**
     * @brief Find state by ID
     * @param id State ID
     * @return Shared pointer to state, or nullptr if not found
     */
    std::shared_ptr<State> findById(UniqueId id) const;

    /**
     * @brief Find state by ID with type checking
     * @tparam T Expected state type
     * @param id State ID
     * @return Shared pointer to state of type T, or nullptr if not found
     */
    template<typename T>
    std::shared_ptr<T> findById(UniqueId id) const {
        static_assert(std::is_base_of_v<State, T>, "T must inherit from State");
        
        auto state = findById(id);
        return std::dynamic_pointer_cast<T>(state);
    }

    /**
     * @brief Get all states of specified type
     * @tparam T State type
     * @return Vector of states of type T
     */
    template<typename T>
    std::vector<std::shared_ptr<T>> getAllOfType() const {
        static_assert(std::is_base_of_v<State, T>, "T must inherit from State");
        
        std::vector<std::shared_ptr<T>> result;
        
        for (const auto& [id, state] : states_) {
            if (auto typedState = std::dynamic_pointer_cast<T>(state)) {
                result.push_back(typedState);
            }
        }
        
        return result;
    }

    /**
     * @brief Get tile state at position
     * @param position Tile position
     * @return Shared pointer to tile state
     */
    std::shared_ptr<TileState> getTileState(const s3d::Point& position) const;

    /**
     * @brief Set tile state at position
     * @param position Tile position
     * @param state Tile state
     */
    void setTileState(const s3d::Point& position, std::shared_ptr<TileState> state);

    /**
     * @brief Get all tile states
     * @return 2D vector of tile states
     */
    const std::vector<std::vector<std::shared_ptr<TileState>>>& getTileGrid() const;

    /**
     * @brief Get cell states in region
     * @param region Rectangle region
     * @return Vector of cell states in region
     */
    std::vector<std::shared_ptr<CellState>> getCellStatesInRegion(const s3d::RectF& region) const;

    /**
     * @brief Get nearest cell state to position
     * @param position World position
     * @param maxDistance Maximum search distance
     * @return Shared pointer to nearest cell state, or nullptr if none found
     */
    std::shared_ptr<CellState> getNearestCellState(const s3d::Vec2& position, double maxDistance = 1000.0) const;

    /**
     * @brief Add cell state at position
     * @param position World position
     * @param cellState Cell state to add
     */
    void addCellState(const s3d::Vec2& position, std::shared_ptr<CellState> cellState);

    /**
     * @brief Remove cell state
     * @param cellState Cell state to remove
     */
    void removeCellState(std::shared_ptr<CellState> cellState);

    /**
     * @brief Get total number of states
     * @return Total state count
     */
    size_t getTotalStateCount() const;

    /**
     * @brief Get number of active cell states
     * @return Active cell count
     */
    size_t getActiveCellCount() const;

    /**
     * @brief Clear all states
     */
    void clear();

    /**
     * @brief Validate all states
     * @return true if all states are valid
     */
    bool validateAllStates() const;

    /**
     * @brief Get states with validation errors
     * @return Vector of invalid states
     */
    std::vector<std::shared_ptr<State>> getInvalidStates() const;

    /**
     * @brief Get grid size
     * @return Grid dimensions
     */
    const s3d::Point& getGridSize() const;

    /**
     * @brief Check if position is within grid bounds
     * @param position Tile position
     * @return true if position is valid
     */
    bool isValidTilePosition(const s3d::Point& position) const;

    /**
     * @brief Convert world position to tile position
     * @param worldPos World position
     * @return Tile position
     */
    s3d::Point worldToTile(const s3d::Vec2& worldPos) const;

    /**
     * @brief Convert tile position to world position
     * @param tilePos Tile position
     * @return World position (center of tile)
     */
    s3d::Vec2 tileToWorld(const s3d::Point& tilePos) const;

    // Object interface
    void save(s3d::JSON& json) const override;
    void load(const s3d::JSON& json) override;
    s3d::String getTypeName() const override;
    std::unique_ptr<core::Object> clone() const override;
    bool isValid() const override;

private:
    /**
     * @brief Initialize tile grid
     */
    void initializeTileGrid();

    /**
     * @brief Update spatial indexing for cell states
     */
    void updateSpatialIndex();

    /**
     * @brief Clean up destroyed states
     */
    void cleanupDestroyedStates();

    // State storage
    std::unordered_map<UniqueId, std::shared_ptr<State>> states_;
    
    // Spatial organization
    s3d::Point gridSize_;
    std::vector<std::vector<std::shared_ptr<TileState>>> tileGrid_;
    
    // Cell state spatial index for fast lookup
    struct CellStateEntry {
        std::shared_ptr<CellState> cellState;
        s3d::Vec2 position;
        s3d::Point tilePosition;
    };
    std::vector<CellStateEntry> cellStates_;
    
    // Performance metrics
    mutable size_t updateCount_;
    mutable double lastUpdateTime_;
    mutable size_t spatialQueryCount_;
    
    // Configuration
    bool isInitialized_;
    double spatialIndexUpdateInterval_;
    double lastSpatialIndexUpdate_;
};

/**
 * @brief RAII guard for state operations
 */
class StateManagerGuard {
public:
    explicit StateManagerGuard(StateManager& manager);
    ~StateManagerGuard();

    StateManagerGuard(const StateManagerGuard&) = delete;
    StateManagerGuard& operator=(const StateManagerGuard&) = delete;

    StateManagerGuard(StateManagerGuard&&) = delete;
    StateManagerGuard& operator=(StateManagerGuard&&) = delete;

    StateManager* get() const;
    StateManager* operator->() const;
    StateManager& operator*() const;

private:
    StateManager* manager_;
};

} // namespace sylife::states