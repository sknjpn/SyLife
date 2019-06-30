#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	s3d::Color		m_color;
	s3d::Polygon	m_polygon;

public:
	void	Draw(double a) const { m_polygon.draw(s3d::ColorF(m_color, a)).drawFrame(1.0, s3d::Palette::Black); }

	void	SetFromJSON(const ptree& pt)
	{
		// color
		m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

		// polygon
		{
			s3d::Array<s3d::Vec2> verticles;

			for (auto v : pt.get_child("verticles"))
				verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));

			m_polygon = s3d::Polygon(verticles);
		}

		Model::SetFromJSON(pt);
	}
	void	Load(const ptree& pt) override { SetFromJSON(pt); }
};

