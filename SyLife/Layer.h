#pragma once

#include "Model.h"

struct Layer
	: public Model
{
	Polygon	m_polygon;
	Color	m_color;

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};
