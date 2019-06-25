#pragma once


#include "Model.h"

#include "Rigidbody.h"

#include "Molecule.h"


class MoleculeModel
	: public Model
{
public:
	double		m_mass;
	double		m_radius;
	s3d::Color	m_color;

public:
	void	SetFromJSON(const ptree& pt);

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};
class MoleculeState
	: public Rigidbody
{
public:
	shared_ptr<MoleculeModel>	m_model;
	
public:
};
