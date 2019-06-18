#include "ShapeModelModel.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

ptree ShapeModel::ToJSON() const
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

			for (const auto& v : m_verticles)
			{
				verticle.put("x", v.x);
				verticle.put("y", v.y);
				
				verticles.push_back(std::make_pair("", verticle));
			}
		}

		pt.add_child("verticles", verticles);
	}

	return pt;
}

void ShapeModel::FromJSON(const ptree& pt)
{
	// color
	{
		auto it = pt.get_child("color").begin();

		m_color = s3d::Color((*it).second.get_value<int>(), (*(it++)).second.get_value<int>(), (*(it++)).second.get_value<int>());
	}

	// verticles
	for (auto v : pt.get_child("verticles"))
		m_verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));

}
