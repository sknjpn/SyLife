#pragma once

#include "sylife/core/common.h"
#include "sylife/core/object.h"

namespace sylife::states {

/**
 * @brief Base class for all game states
 */
class State : public core::Object {
public:
    /**
     * @brief State lifecycle
     */
    enum class Lifecycle {
        CREATED,    ///< Just created, not yet initialized
        ACTIVE,     ///< Active and updating
        PAUSED,     ///< Paused, not updating but still valid
        DESTROYED   ///< Destroyed, should be cleaned up
    };

    /**
     * @brief Constructor
     */
    State();

    /**
     * @brief Destructor
     */
    ~State() override;

    /**
     * @brief Initialize the state
     */
    virtual void initialize();

    /**
     * @brief Update the state
     * @param deltaTime Time step
     */
    virtual void update(double deltaTime);

    /**
     * @brief Render the state (for debug visualization)
     */
    virtual void render() const;

    /**
     * @brief Activate the state
     */
    virtual void activate();

    /**
     * @brief Pause the state
     */
    virtual void pause();

    /**
     * @brief Destroy the state
     */
    virtual void destroy();

    /**
     * @brief Get state lifecycle
     * @return Current lifecycle
     */
    Lifecycle getLifecycle() const;

    /**
     * @brief Check if state is active
     * @return true if active
     */
    bool isActive() const;

    /**
     * @brief Check if state is destroyed
     * @return true if destroyed
     */
    bool isDestroyed() const;

    /**
     * @brief Set position in world
     * @param position World position
     */
    void setPosition(const s3d::Vec2& position);

    /**
     * @brief Get position in world
     * @return World position
     */
    const s3d::Vec2& getPosition() const;

    /**
     * @brief Set rotation
     * @param rotation Rotation in radians
     */
    void setRotation(double rotation);

    /**
     * @brief Get rotation
     * @return Rotation in radians
     */
    double getRotation() const;

    /**
     * @brief Set scale
     * @param scale Scale factor
     */
    void setScale(const s3d::Vec2& scale);

    /**
     * @brief Get scale
     * @return Scale factor
     */
    const s3d::Vec2& getScale() const;

    /**
     * @brief Get age of state
     * @return Age in seconds
     */
    double getStateAge() const;

    /**
     * @brief Reset age counter
     */
    void resetAge();

    // Object interface
    void save(s3d::JSON& json) const override;
    void load(const s3d::JSON& json) override;
    s3d::String getTypeName() const override;
    std::unique_ptr<core::Object> clone() const override;
    bool isValid() const override;

protected:
    /**
     * @brief Called when state is activated
     */
    virtual void onActivate() {}

    /**
     * @brief Called when state is paused
     */
    virtual void onPause() {}

    /**
     * @brief Called when state is destroyed
     */
    virtual void onDestroy() {}

    /**
     * @brief Called during update (override for custom behavior)
     * @param deltaTime Time step
     */
    virtual void onUpdate(double deltaTime) {}

    /**
     * @brief Called during render (override for custom behavior)
     */
    virtual void onRender() const {}

private:
    // State properties
    Lifecycle lifecycle_;
    s3d::Vec2 position_;
    double rotation_;
    s3d::Vec2 scale_;
    
    // Timing
    std::chrono::high_resolution_clock::time_point stateCreationTime_;
    double accumulatedTime_;
    bool isPaused_;
};

/**
 * @brief Get string representation of lifecycle
 * @param lifecycle Lifecycle enum
 * @return String representation
 */
const char* lifecycleToString(State::Lifecycle lifecycle);

/**
 * @brief Parse lifecycle from string
 * @param str String representation
 * @return Lifecycle enum
 */
State::Lifecycle stringToLifecycle(const std::string& str);

} // namespace sylife::states