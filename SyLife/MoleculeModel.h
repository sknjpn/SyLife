#pragma once

#include "Model.h"

class MoleculeModel
	: public Model
{
public:
	double	m_mass;
	double	m_radius;
	int		m_color[3];

public:
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/molecule/" + GetFilename(); }
};
