#pragma once

#include "Model.h"

class CellModel
	: public Model
{
public:
	string	m_name;

public:
	ptree	ToJSON() const override;
	void	FromJSON(const ptree& pt) override;

	string	GetFilepath() const override { return "assets/part/cell/" + GetFilename(); }
};