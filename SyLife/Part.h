#pragma once

#include "Shape.h"

class Part
{
public:
	class Model
	{
	public:
		double		m_mass;
		double		m_inertia;
		string		m_name;
		Shape		m_shape;
		Vector2D	m_lu;
		Vector2D	m_rd;
	};

	double		m_rotation;
	Vector2D	m_position;
	shared_ptr<Model>	m_model;

public:
	Part();
	~Part();

	void	Draw();
};
