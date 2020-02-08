#pragma once

#include "Model.h"

#include "Layer.h"

class Shape
	: public Model
	, public Array<Layer>
{
	// 合成されたもの
	Polygon	m_polygon;
	Texture	m_preRenderTexture;

public:
	void	preRender();
	bool	updateProperties();

	const Polygon& getPolygon() const { return m_polygon; }

	double	getInertia(double mass) const;
	double	getRadius() const { return Sqrt(2 * getInertia(1.0)); }
	Vec2	getCentroid() const { return m_polygon.centroid(); }

	RectF	getTileSize() const;

	void	draw(double a) const { m_preRenderTexture.scaled(1.0 / GeneralSetting::GetInstance().m_textureScale).draw(m_polygon.boundingRect().pos, ColorF(1.0 ,a)); }

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};

