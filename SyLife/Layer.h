#pragma once

#include "Model.h"

struct Layer
	: public Model
{
	Polygon	m_polygon;
	Color	m_color;

	RectF	getRect() const;

	void	load(const ptree& pt) override;
	void	save(ptree& pt) const override;
};
