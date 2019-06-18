#include "ModuleModel.h"
#include <boost/property_tree/json_parser.hpp>

void ModuleModel::FromJSON(const ptree& pt)
{
	// name
	m_name = pt.get<string>("name");

	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
	{
		m_shapes.emplace_back().FromJSON(shape.second);
	}
}