#pragma once

#include "Model.h"

class MoleculeModel
	: public Model
{
public:
	string	m_name;
	double	m_mass;
	double	m_radius;
	char	m_color[3];

public:
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/molecule/" + GetFilename(); }
};