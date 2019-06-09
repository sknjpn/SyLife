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
};
