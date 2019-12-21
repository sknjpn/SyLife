#pragma once

#include "Model.h"

struct Layer
	: public Model
{
	Polygon	m_polygon;
	Color	m_color;

	void	load(const JSONValue& json) override;
	void	save(JSONWriter& json) const override;
};
