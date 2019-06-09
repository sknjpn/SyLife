#pragma once

#include "Model.h"

class MoleculeModel
	: public Model
{
public:
	string	m_name;
	string	m_filepath;
	double	m_mass;
	double	m_radius;
	char	m_color[3];

public:
	ptree	ToJSON() const override;
	void	Load(const string& path) const override;
	void	Save() const override;
};
