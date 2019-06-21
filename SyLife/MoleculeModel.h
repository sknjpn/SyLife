#pragma once

#include "Model.h"

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
