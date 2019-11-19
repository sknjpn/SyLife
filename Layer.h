#pragma once

#include "Model.h"

struct Layer
	: public Model
{
	Polygon	m_polygon;
	Color	m_color;

	RectF	getRect() const;

	void	load(const JSONValue& json) override;
	void	save(const JSONWriter& json) const override;
};
