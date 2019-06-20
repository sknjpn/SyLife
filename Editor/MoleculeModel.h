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
	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/molecule/" + GetFilename(); }
};
