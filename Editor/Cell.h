#pragma once

#include "Model.h"

class CellModel
	: public Model
{
public:
	string	m_name;
	string	m_filepath;

public:
	ptree	ToJSON() const override;
	void	Load(const string& filepath) override;
	void	Save() const override;
};