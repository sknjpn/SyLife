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

	double	getInertia(double mass) const;
	double	getRadius() const { return Sqrt(2 * getInertia(1.0)); }
	Vec2	getCentroid() const { return m_polygon.centroid(); }

	RectF	getTileSize() const;

	void	draw(double a) const { for (const auto& l : *this) l.m_polygon.draw(ColorF(l.m_color, a)); }

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};

