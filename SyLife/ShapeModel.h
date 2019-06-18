#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	char				m_color[3];
	vector<Vector2D>	m_verticles;

public:
	void	FromJSON(const ptree& pt) override;
};

