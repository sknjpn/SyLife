#pragma once

#include "Particle.h"

class Rigidbody
	: public Particle
{
	double	m_radius = 0.0;
	double	m_rotation = 0.0;
	double	m_inertia = 0.0;
	double	m_angularVelocity = 0.0;

public:
	virtual ~Rigidbody() = default;

	// Get
	double	GetRadius() const { return m_radius; }
	double	GetRotation() const { return m_rotation; }
	double	GetInertia() const { return m_inertia; }
	double	GetAngularVelocity() const { return m_angularVelocity; }

	// Set
	void	SetRadius(double radius) { m_radius = radius; }
	void	SetRotation(double rotation) { m_rotation = rotation; }
	void	SetInertia(double inertia) { m_inertia = inertia; }
	void	SetAngularVelocity(double angularVelocity) { m_angularVelocity = angularVelocity; }

	void	Init()
	{
		// Šp‰^“®—Ê‚ÌŒvŽZ
		m_inertia = GetMass() * m_radius * m_radius * 0.5;
	}

	Vector2D	GetVelocityAt(const Vector2D& position) const { return GetVelocity() + (position - GetPosition()).rotated(m_rotation) * m_angularVelocity; }
	Vector2D	GetWorldPosition(const Vector2D& localPosition) const { return GetPosition() + localPosition.rotated(m_rotation); }

	void	AddForceInWorld(const Vector2D& force, const Vector2D& worldPosition) { AddImpulseInWorld(force * g_fieldManagerPtr->GetDeltaTime(), worldPosition); }
	void	AddForceInLocal(const Vector2D& force, const Vector2D& localPosition) { AddForceInWorld(force.rotated(m_rotation), GetWorldPosition(localPosition)); }
	void	AddImpulseInLocal(const Vector2D& impulse, const Vector2D& localPosition) { AddImpulseInWorld(impulse.rotated(m_rotation), GetWorldPosition(localPosition)); }
	void	AddImpulseInWorld(const Vector2D& impulse, const Vector2D& worldPosition);

	void	UpdateRigidbody();
};
