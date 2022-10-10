#pragma once

#include "Particle.h"

class Rigidbody : public Particle
{
  double m_radius = 1.0;
  double m_rotation = 0.0;
  double m_inertia = 1.0;
  double m_angularVelocity = 0.0;

public:
  virtual ~Rigidbody() = default;

  // Get
  double getRadius() const { return m_radius; }
  double getRotation() const { return m_rotation; }
  double getInertia() const { return m_inertia; }
  double getAngularVelocity() const { return m_angularVelocity; }

  Mat3x2 getMat3x2() const
  {
    return Mat3x2::Rotate(getRotation()).translated(getPosition());
  }

  // Set
  void setRadius(double radius) { m_radius = radius; }
  void setRotation(double rotation) { m_rotation = rotation; }
  void setInertia(double inertia) { m_inertia = inertia; }
  void setAngularVelocity(double angularVelocity)
  {
    m_angularVelocity = angularVelocity;
  }

  void init()
  {
    // 角運動量の計算
    m_inertia = getMass() * m_radius * m_radius * 0.5;
  }

  Vec2 getVelocityAt(const Vec2& position) const
  {
    return getVelocity() + (position - getPosition()).rotated(m_rotation) * m_angularVelocity;
  }
  Vec2 getWorldPosition(const Vec2& localPosition) const
  {
    return getPosition() + localPosition.rotated(m_rotation);
  }

  void addForceInWorld(const Vec2& force, const Vec2& worldPosition);
  void addForceInLocal(const Vec2& force, const Vec2& localPosition)
  {
    addForceInWorld(force.rotated(m_rotation), getWorldPosition(localPosition));
  }
  void addImpulseInLocal(const Vec2& impulse, const Vec2& localPosition)
  {
    addImpulseInWorld(impulse.rotated(m_rotation), getWorldPosition(localPosition));
  }
  void addImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition);

  void updateRigidbody();

  void load(Deserializer<BinaryReader>& reader);
  void save(Serializer<MemoryWriter>& writer) const;
};
