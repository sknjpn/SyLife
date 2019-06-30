#pragma once

#include "FieldManager.h"

class Particle
{
	bool		m_isDestroyed = false;
	double		m_mass = 0.0;
	Vector2D	m_position = Vector2D(0.0, 0.0);
	Vector2D	m_velocity = Vector2D(0.0, 0.0);

public:
	virtual ~Particle() = default;

	// Get
	bool	IsDestroyed() const { return m_isDestroyed; }
	double	GetMass() const { return m_mass; }
	const Vector2D&	GetPosition() const { return m_position; }
	const Vector2D&	GetVelocity() const { return m_velocity; }

	// Set
	void	Destroy() { m_isDestroyed = true; }
	void	SetMass(double mass) { m_mass = mass; }
	void	SetPosition(const Vector2D& position) { m_position = position; }
	void	SetVelocity(const Vector2D& velocity) { m_velocity = velocity; }
	
	void	AddForce(const Vector2D& force) { m_velocity += force / m_mass * g_fieldManagerPtr->GetDeltaTime(); }
	void	AddImpulse(const Vector2D& force) { m_velocity += force / m_mass; }
	
	void	UpdateParticle();
};

