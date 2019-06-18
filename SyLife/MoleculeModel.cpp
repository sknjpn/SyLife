#include "MoleculeModel.h"
#include <boost/property_tree/json_parser.hpp>

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
