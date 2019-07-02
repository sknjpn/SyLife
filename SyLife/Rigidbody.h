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

	Vec2	GetVelocityAt(const Vec2& position) const { return GetVelocity() + (position - GetPosition()).rotated(m_rotation) * m_angularVelocity; }
	Vec2	GetWorldPosition(const Vec2& localPosition) const { return GetPosition() + localPosition.rotated(m_rotation); }

	void	AddForceInWorld(const Vec2& force, const Vec2& worldPosition) { AddImpulseInWorld(force * g_fieldManagerPtr->GetDeltaTime(), worldPosition); }
	void	AddForceInLocal(const Vec2& force, const Vec2& localPosition) { AddForceInWorld(force.rotated(m_rotation), GetWorldPosition(localPosition)); }
	void	AddImpulseInLocal(const Vec2& impulse, const Vec2& localPosition) { AddImpulseInWorld(impulse.rotated(m_rotation), GetWorldPosition(localPosition)); }
	void	AddImpulseInWorld(const Vec2& impulse, const Vec2& worldPosition);

	void	UpdateRigidbody();
};
