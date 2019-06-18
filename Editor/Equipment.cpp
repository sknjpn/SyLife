#include "Equipment.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

ptree EquipmentModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put("name", m_name);

	// type
	pt.put("type", "Equipment");

	// mass
	pt.put<int>("mass", m_mass);

	// shapes
	{
		ptree shapes;

		for (const auto& v : m_shapes)
		{
			shapes.push_back(std::make_pair("", v.ToJSON()));
		}

		pt.add_child("shapes", shapes);
	}

	return pt;
}

void EquipmentModel::FromJSON(const ptree& pt) const
{
	// name
	m_name = pt.get<string>("name");

	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
	{
		auto& s = m_shapes.emplace_back();

		// color
		{
			auto it = shape.second.get_child("color").begin();
			s.m_color = s3d::Color((*it).second.get_value<int>(), (*(it++)).second.get_value<int>(), (*(it++)).second.get_value<int>());
		}

		// verticles
		for (auto v : shape.second.get_child("verticles"))
			s.m_verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));
	}
}
