#include "ShapeModel.h"
#include <boost/property_tree/json_parser.hpp>

ptree ShapeModel::ToJSON() const
{
	ptree pt;

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
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// verticles
	for (auto v : pt.get_child("verticles"))
		m_verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));
}
