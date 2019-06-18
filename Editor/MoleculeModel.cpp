#include "MoleculeModel.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

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
		ptree color_r, color_g, color_b;

		color_r.put("", m_color[0]);
		color_g.put("", m_color[1]);
		color_b.put("", m_color[2]);

		color.push_back(std::make_pair("", color_r));
		color.push_back(std::make_pair("", color_g));
		color.push_back(std::make_pair("", color_b));

		pt.add_child("color", color);
	}

	return pt;
}

void MoleculeModel::FromJSON(const ptree& pt)
{
	// color
	{
		auto child = pt.get_child("color");
		int n = 0;

		for (auto it = child.begin(); n != 3; ++it)
		{
			m_color[n] = (*it).second.get_value<int>();

			++n;
		}
	}

	// mass
	m_mass = pt.get<double>("mass");

	// name
	m_name = pt.get<string>("name");

	// radius
	m_radius = sqrt(m_mass);
}
