#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	Color		m_color;
	Polygon	m_polygon;

public:
	void	Draw(double a) const { m_polygon.draw(ColorF(m_color, a)).drawFrame(1.0, Palette::Black); }

	void	SetFromJSON(const ptree& pt)
	{
		// color
		m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

		// polygon
		{
			Array<Vec2> verticles;

			for (auto v : pt.get_child("verticles"))
				verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));

			m_polygon = Polygon(verticles);
		}

		Model::SetFromJSON(pt);
	}
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

