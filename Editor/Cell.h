#pragma once

#include "Model.h"

class CellModel
	: public Model
{
public:
	string	m_name;

public:
	ptree	ToJSON() const override;
	void	Load(const string& filepath) override;

	string	GetFilepath() const override { return "asset/part/cell/" + GetFilename(); }
};