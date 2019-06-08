#pragma once

#include "Rigidbody.h"

class MoleculeModel
{
public:
	string	m_name;
	double	m_mass;
	double	m_radius;
	char	m_color[3];
};

class Molecule : public Rigidbody
{
public:
	shared_ptr<MoleculeModel>	m_model;
	
public:
};
