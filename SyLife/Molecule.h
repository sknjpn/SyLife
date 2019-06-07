#pragma once

#include "Rigidbody.h"

class MoleculeModel
{
public:
	string	m_name;
	double	m_mass;
	double	m_radius;
};

class Molecule : public Rigidbody
{
public:

	shared_ptr<MoleculeModel>	m_model;
	
public:
	Molecule();
	~Molecule();
};
