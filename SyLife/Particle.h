#pragma once

class Particle
{
public:
	bool		m_isDestroyed = false;
	double		m_mass = 0.0;
	Vector2D	m_position = Vector2D(0.0, 0.0);
	Vector2D	m_velocity = Vector2D(0.0, 0.0);

public:
	virtual ~Particle() = default;
};

