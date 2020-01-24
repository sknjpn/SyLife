#pragma once

#include "common.h"

class Particle
{
	bool	m_isDestroyed = false;
	double	m_mass = 1.0;
	Vec2	m_position = Vec2(0.0, 0.0);
	Vec2	m_velocity = Vec2(0.0, 0.0);

public:
	virtual ~Particle() = default;

	// Get
	bool	isDestroyed() const { return m_isDestroyed; }
	double	getMass() const { return m_mass; }
	const Vec2&	getPosition() const { return m_position; }
	const Vec2&	getVelocity() const { return m_velocity; }

	// Set
	void	destroy() { m_isDestroyed = true; }
	void	setMass(double mass) { m_mass = mass; }
	void	setPosition(const Vec2& position) { m_position = position; }
	void	setVelocity(const Vec2& velocity) { m_velocity = velocity; }
	
	void	addForce(const Vec2& force);
	void	addImpulse(const Vec2& force) { m_velocity += force / m_mass; }
	
	void	updateParticle();

	void	load(Deserializer<ByteArray>& reader);
	void	save(Serializer<MemoryWriter>& writer) const;
};
