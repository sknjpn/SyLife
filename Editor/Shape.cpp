#include "Shape.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

ptree Shape::ToJSON() const
{
	ptree pt;

	// color
	{
		ptree color;
		ptree color_r, color_g, color_b;

		color_r.put("", m_color.toColor().r);
		color_g.put("", m_color.toColor().g);
		color_b.put("", m_color.toColor().b);

		color.push_back(std::make_pair("", color_r));
		color.push_back(std::make_pair("", color_g));
		color.push_back(std::make_pair("", color_b));

		pt.add_child("color", color);
	}

	// verticles
	{
		ptree verticles;

		{
			ptree verticle;
			ptree verticle_x, verticle_y;

			for (const auto& v : m_verticles)
			{
				verticle_x.put("", v.x);
				verticle_y.put("", v.y);

				verticle.push_back(std::make_pair("", verticle_x));
				verticle.push_back(std::make_pair("", verticle_y));
				
				verticles.push_back(std::make_pair("", verticle));
			}
		}

		pt.add_child("verticles", verticles);
	}

	return pt;
}

void Shape::Load(const string& filepath)
{
}

void Shape::Save() const
{
}
