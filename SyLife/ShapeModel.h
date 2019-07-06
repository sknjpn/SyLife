#pragma once

#include "Model.h"

class ShapeModel
	: public Model
{
public:
	Color	m_color;
	Polygon	m_polygon;

public:
	// Get
	const Color&	GetColor() const { return m_color; }
	const Polygon&	GetPolygon() const { return m_polygon; }
	Polygon&	GetPolygon() { return m_polygon; }

	void	Draw(double a) const
	{
		m_polygon
			.draw(ColorF(m_color, a));
			//.drawFrame(1.0, Palette::Black); 
	}

	void	Load_this(const ptree& pt)
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

		Model::Load_this(pt);
	}
	void	Load(const ptree& pt) override { Load_this(pt); }
	void	Save_this(ptree& pt) const
	{
		// color
		{
			ptree color;

			color.put("r", m_color.r);
			color.put("g", m_color.g);
			color.put("b", m_color.b);

			pt.add_child("color", color);
		}

		// verticles
		{
			ptree verticles;

			{
				ptree verticle;

				for (const auto& v : m_polygon.vertices())
				{
					verticle.put("x", v.x);
					verticle.put("y", v.y);

					verticles.push_back(std::make_pair("", verticle));
				}
			}

			pt.add_child("verticles", verticles);
		}

		Model::Save_this(pt);

		// type
		pt.put("type", "ShapeModel");
	}
	void	Save(ptree& pt) const override { Save_this(pt); }
};

