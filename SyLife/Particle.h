#pragma once

class Particle
{
public:
	bool	m_destroyFlag = false;
	double	m_mass = 0.0;
	Vector2D	m_position;
	Vector2D	m_velocity;

public:
	Particle();
	~Particle();
};

