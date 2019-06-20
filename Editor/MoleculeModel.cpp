#include "MoleculeModel.h"
#include <boost/property_tree/json_parser.hpp>

ptree MoleculeModel::ToJSON() const
{
	ptree pt;

	// name
	pt.put<string>("name", m_name);

	// mass
	pt.put<int>("mass", m_mass);

	// color
	{
		ptree color;

		color.put("r", m_color.r);
		color.put("g", m_color.g);
		color.put("b", m_color.b);

		pt.add_child("color", color);
	}

	return pt;
}

void MoleculeModel::FromJSON(const ptree& pt)
{
	// color
	m_color = s3d::Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// mass
	m_mass = pt.get<double>("mass");

	// name
	m_name = pt.get<string>("name");

	// radius
	m_radius = sqrt(m_mass);
}
