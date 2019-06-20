#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	s3d::Color		m_color;
	s3d::Polygon	m_polygon;

public:
	void	FromJSON(const ptree& pt) override;

	void	Draw(double a) const { m_polygon.draw(s3d::ColorF(m_color, a)); }
};

