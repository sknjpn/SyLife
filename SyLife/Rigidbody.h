#pragma once

#include "Particle.h"
#include "nanoflann.hpp"

class Rigidbody 
	: public Particle
{
public:
	double	m_radius = 0.0;
	double	m_rotation = 0.0;
	double	m_inertia = 0.0;
	double	m_angularVelocity = 0.0;

public:
	Rigidbody() = default;
	virtual ~Rigidbody() = default;

	void	Init();

	Vector2D	GetVelocity(const Vector2D& position) const;

	Vector2D	GetWorldPosition(const Vector2D& localPosition) const { return m_position + localPosition.rotated(m_rotation); }

	void	AddForceInWorld(const Vector2D& force, const Vector2D& worldPosition);
	void	AddForceInLocal(const Vector2D& force, const Vector2D& localPosition) { AddForceInWorld(force.rotated(m_rotation), GetWorldPosition(localPosition)); }

	void	AddImpulseInWorld(const Vector2D& impulse, const Vector2D& worldPosition);
	void	AddImpulseInLocal(const Vector2D& impulse, const Vector2D& localPosition) { AddImpulseInWorld(impulse.rotated(m_rotation), GetWorldPosition(localPosition)); }
};
