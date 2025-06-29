#pragma once

#include "../types.h"
#include <cmath>
#include <algorithm>

namespace sylife::ecs {

/**
 * @brief 2D velocity component for moving entities
 */
struct VelocityComponent final : public Component<VelocityComponent> {
    float vx = 0.0f;  // Velocity in X direction
    float vy = 0.0f;  // Velocity in Y direction
    float vz = 0.0f;  // Velocity in Z direction
    
    float max_speed = 100.0f;  // Maximum speed limit
    float drag = 0.0f;         // Air resistance/drag coefficient (0.0 = no drag)
    
    /**
     * @brief Default constructor
     */
    VelocityComponent() = default;
    
    /**
     * @brief Constructor with 2D velocity
     * @param vx X velocity
     * @param vy Y velocity
     * @param vz Z velocity (default 0)
     * @param max_speed Maximum speed limit (default 100)
     * @param drag Drag coefficient (default 0)
     */
    VelocityComponent(float vx, float vy, float vz = 0.0f, 
                     float max_speed = 100.0f, float drag = 0.0f)
        : vx(vx), vy(vy), vz(vz), max_speed(max_speed), drag(drag) {}
    
    /**
     * @brief Get current speed (magnitude of velocity)
     * @return Current speed
     */
    float getSpeed() const {
        return std::sqrt(vx * vx + vy * vy + vz * vz);
    }
    
    /**
     * @brief Get current 2D speed (ignoring Z)
     * @return Current 2D speed
     */
    float getSpeed2D() const {
        return std::sqrt(vx * vx + vy * vy);
    }
    
    /**
     * @brief Get squared speed (faster than getSpeed)
     * @return Squared speed
     */
    float getSpeedSquared() const {
        return vx * vx + vy * vy + vz * vz;
    }
    
    /**
     * @brief Get direction angle in radians (2D)
     * @return Direction angle in radians
     */
    float getDirection() const {
        return std::atan2(vy, vx);
    }
    
    /**
     * @brief Set velocity from speed and direction
     * @param speed Speed magnitude
     * @param direction Direction in radians
     */
    void setFromPolar(float speed, float direction) {
        vx = speed * std::cos(direction);
        vy = speed * std::sin(direction);
    }
    
    /**
     * @brief Add acceleration to velocity
     * @param ax X acceleration
     * @param ay Y acceleration
     * @param az Z acceleration (default 0)
     * @param delta_time Time step
     */
    void accelerate(float ax, float ay, float az = 0.0f, float delta_time = 1.0f) {
        vx += ax * delta_time;
        vy += ay * delta_time;
        vz += az * delta_time;
        clampSpeed();
    }
    
    /**
     * @brief Apply drag/air resistance
     * @param delta_time Time step
     */
    void applyDrag(float delta_time) {
        if (drag > 0.0f) {
            float drag_factor = std::max(0.0f, 1.0f - drag * delta_time);
            vx *= drag_factor;
            vy *= drag_factor;
            vz *= drag_factor;
        }
    }
    
    /**
     * @brief Clamp velocity to maximum speed
     */
    void clampSpeed() {
        if (max_speed > 0.0f) {
            float current_speed = getSpeed();
            if (current_speed > max_speed) {
                float scale = max_speed / current_speed;
                vx *= scale;
                vy *= scale;
                vz *= scale;
            }
        }
    }
    
    /**
     * @brief Normalize velocity to unit length
     */
    void normalize() {
        float speed = getSpeed();
        if (speed > 0.0f) {
            vx /= speed;
            vy /= speed;
            vz /= speed;
        }
    }
    
    /**
     * @brief Set velocity magnitude while preserving direction
     * @param new_speed New speed magnitude
     */
    void setSpeed(float new_speed) {
        float current_speed = getSpeed();
        if (current_speed > 0.0f) {
            float scale = new_speed / current_speed;
            vx *= scale;
            vy *= scale;
            vz *= scale;
        }
    }
    
    /**
     * @brief Rotate velocity by angle (2D)
     * @param angle Rotation angle in radians
     */
    void rotate(float angle) {
        float cos_angle = std::cos(angle);
        float sin_angle = std::sin(angle);
        
        float new_vx = vx * cos_angle - vy * sin_angle;
        float new_vy = vx * sin_angle + vy * cos_angle;
        
        vx = new_vx;
        vy = new_vy;
    }
    
    /**
     * @brief Reflect velocity off a surface normal
     * @param normal_x Surface normal X component
     * @param normal_y Surface normal Y component
     * @param restitution Bounce coefficient (0 = no bounce, 1 = perfect bounce)
     */
    void reflect(float normal_x, float normal_y, float restitution = 1.0f) {
        // Normalize the normal vector
        float normal_length = std::sqrt(normal_x * normal_x + normal_y * normal_y);
        if (normal_length > 0.0f) {
            normal_x /= normal_length;
            normal_y /= normal_length;
            
            // Calculate dot product
            float dot = vx * normal_x + vy * normal_y;
            
            // Reflect velocity
            vx -= 2.0f * dot * normal_x * restitution;
            vy -= 2.0f * dot * normal_y * restitution;
        }
    }
    
    /**
     * @brief Dampen velocity (reduce by factor)
     * @param factor Damping factor (0.0 = stop completely, 1.0 = no damping)
     */
    void dampen(float factor) {
        vx *= factor;
        vy *= factor;
        vz *= factor;
    }
    
    /**
     * @brief Stop all movement
     */
    void stop() {
        vx = vy = vz = 0.0f;
    }
    
    /**
     * @brief Check if entity is moving
     * @param threshold Minimum speed threshold
     * @return true if speed is above threshold
     */
    bool isMoving(float threshold = 0.001f) const {
        return getSpeedSquared() > threshold * threshold;
    }
    
    /**
     * @brief Get velocity as array
     * @return Array containing [vx, vy, vz]
     */
    std::array<float, 3> toArray() const {
        return {vx, vy, vz};
    }
    
    /**
     * @brief Linear interpolation between velocities
     * @param other Target velocity
     * @param t Interpolation factor (0.0 to 1.0)
     * @return Interpolated velocity
     */
    VelocityComponent lerp(const VelocityComponent& other, float t) const {
        return VelocityComponent(
            vx + (other.vx - vx) * t,
            vy + (other.vy - vy) * t,
            vz + (other.vz - vz) * t,
            max_speed + (other.max_speed - max_speed) * t,
            drag + (other.drag - drag) * t
        );
    }
    
    // Operator overloads for convenience
    VelocityComponent operator+(const VelocityComponent& other) const {
        return VelocityComponent(vx + other.vx, vy + other.vy, vz + other.vz, 
                               std::max(max_speed, other.max_speed), drag);
    }
    
    VelocityComponent operator-(const VelocityComponent& other) const {
        return VelocityComponent(vx - other.vx, vy - other.vy, vz - other.vz, 
                               max_speed, drag);
    }
    
    VelocityComponent operator*(float scalar) const {
        return VelocityComponent(vx * scalar, vy * scalar, vz * scalar, 
                               max_speed, drag);
    }
    
    VelocityComponent operator/(float scalar) const {
        return VelocityComponent(vx / scalar, vy / scalar, vz / scalar, 
                               max_speed, drag);
    }
    
    VelocityComponent& operator+=(const VelocityComponent& other) {
        vx += other.vx;
        vy += other.vy;
        vz += other.vz;
        clampSpeed();
        return *this;
    }
    
    VelocityComponent& operator-=(const VelocityComponent& other) {
        vx -= other.vx;
        vy -= other.vy;
        vz -= other.vz;
        return *this;
    }
    
    VelocityComponent& operator*=(float scalar) {
        vx *= scalar;
        vy *= scalar;
        vz *= scalar;
        clampSpeed();
        return *this;
    }
    
    VelocityComponent& operator/=(float scalar) {
        vx /= scalar;
        vy /= scalar;
        vz /= scalar;
        return *this;
    }
    
    bool operator==(const VelocityComponent& other) const {
        return vx == other.vx && vy == other.vy && vz == other.vz &&
               max_speed == other.max_speed && drag == other.drag;
    }
    
    bool operator!=(const VelocityComponent& other) const {
        return !(*this == other);
    }
};

} // namespace sylife::ecs