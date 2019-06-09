#pragma once

#include "Model.h"

class PartModel
	: public Model
{
public:
	string	m_name;
	string	m_filepath;
	double	m_mass;

	ptree	ToJson() const override;
};
