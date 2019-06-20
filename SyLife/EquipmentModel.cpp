#include "EquipmentModel.h"
#include <boost/property_tree/json_parser.hpp>

void EquipmentModel::FromJSON(const ptree& pt)
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

	// approximateRect
	{
		const auto& p = m_shapes.front().m_polygon.vertices().front();

		m_approximateRect = { {p.x, p.y}, {p.x, p.y} };

		for (const auto& s : m_shapes)
		{
			for (const auto& v : s.m_polygon.vertices())
			{
				if (m_approximateRect.first.m_x > v.x) m_approximateRect.first.m_x = v.x;
				if (m_approximateRect.first.m_y > v.y) m_approximateRect.first.m_y = v.y;
				if (m_approximateRect.second.m_x < v.x) m_approximateRect.second.m_x = v.x;
				if (m_approximateRect.second.m_y < v.y) m_approximateRect.second.m_y = v.y;
			}
		}
	}
}
