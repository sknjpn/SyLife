#include "CellModel.h"
#include <boost/property_tree/json_parser.hpp>


void CellModel::FromJSON(const ptree& pt)
{
	// name
	m_name = pt.get<string>("name");

	// body
	m_body->FromJSON(pt.get_child("body"));

	// equipments
	for (auto equipment : pt.get_child("equipments"))
		m_equipments.emplace_back(make_shared<EquipmentConfig>(equipment.second));

	// modules
	for (auto module : pt.get_child("modules"))
		m_modules.emplace_back(make_shared<ModuleConfig>(module.second));
}

void CellModel::CalculateDisk()
{
	// mass
	{
		m_mass = 0.0;

		// body
		double result = m_body->m_model->m_mass;

		// equipment
		for (const auto& e : m_equipments) result += e->m_model->m_mass;

		// module
		for (const auto& m : m_modules) result += m->m_model->m_mass;
	}

	// center
	{
		// body
		Vector2D center = m_body->m_model->m_mass * (m_body->m_model->m_approximateRect.first + m_body->m_model->m_approximateRect.second) / 2.0;

		// equipment
		for (const auto& e : m_equipments)
			center += e->m_model->m_mass * (e->m_position + ((e->m_model->m_approximateRect.first + e->m_model->m_approximateRect.second) / 2.0).rotated(e->m_rotation));

		// module
		for (const auto& m : m_modules)
			center += m->m_model->m_mass * (m->m_position + ((m->m_model->m_approximateRect.first + m->m_model->m_approximateRect.second) / 2.0).rotated(m->m_rotation));

		center /= m_mass;

		// à íuí≤êÆ

	}
}
