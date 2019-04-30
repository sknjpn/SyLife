#pragma once

#include "Rigidbody.h"

class Molecule : public Rigidbody
{
public:
	class Model
	{
	public:
		string	m_name;
		double	m_mass;
		double	m_radius;
	};

	shared_ptr<Model>	m_model;
	
public:
	Molecule();
	~Molecule();
};
