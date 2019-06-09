#pragma once

#include "Model.h"

class CellModel
	: public Model
{
public:
	string	m_name;
	string	m_filepath;

public:
	ptree	ToJson() const override;
};