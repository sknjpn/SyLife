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

	string	GetFilepath() const override { return "assets/molecule/" + GetFilename(); }

	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};
class Molecule
	: public Rigidbody
{
public:
	shared_ptr<MoleculeModel>	m_model;
	
public:
};
