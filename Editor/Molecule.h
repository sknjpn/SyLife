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
	ptree	ToJSON() const override;
	void	Load(const string& filepath) override;
	void	Save() const override;
};
