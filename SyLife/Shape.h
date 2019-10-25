#pragma once

#include "Model.h"

#include "Layer.h"

class Shape
	: public Model
	, public Array<Layer>
{
	// 合成されたもの
	Polygon	m_polygon;

public:
	bool	updateProperties();

	const Polygon& getPolygon() const { return m_polygon; }
	double	getRadius() const { return m_radius; }

	void	load_this(const ptree& pt);
	void	load(const ptree& pt) override { load_this(pt); }
	void	save_this(ptree& pt) const;
	void	save(ptree& pt) const override { save_this(pt); }
};

