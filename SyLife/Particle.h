#pragma once

#include "FieldManager.h"

class Particle
{
	bool		m_isDestroyed = false;
	double		m_mass = 0.0;
	s3d::Vec2	m_position = s3d::Vec2(0.0, 0.0);
	s3d::Vec2	m_velocity = s3d::Vec2(0.0, 0.0);

public:
	virtual ~Particle() = default;

	// Get
	bool	IsDestroyed() const { return m_isDestroyed; }
	double	GetMass() const { return m_mass; }
	const s3d::Vec2&	GetPosition() const { return m_position; }
	const s3d::Vec2&	GetVelocity() const { return m_velocity; }

	// Set
	void	Destroy() { m_isDestroyed = true; }
	void	SetMass(double mass) { m_mass = mass; }
	void	SetPosition(const s3d::Vec2& position) { m_position = position; }
	void	SetVelocity(const s3d::Vec2& velocity) { m_velocity = velocity; }
	
	void	AddForce(const s3d::Vec2& force) { m_velocity += force / m_mass * g_fieldManagerPtr->GetDeltaTime(); }
	void	AddImpulse(const s3d::Vec2& force) { m_velocity += force / m_mass; }
	
	void	UpdateParticle();
};

