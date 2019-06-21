#include "CellModel.h"

void CellModel::SetFromJSON(const ptree & pt)
{
	// parts
	for (auto part : pt.get_child("parts"))
	{
		shared_ptr<PartConfig> pc;

		if (part.second.get<string>("type") == "Body") m_parts.emplace_back(m_body = make_shared<BodyConfig>())->Load(part.second);
		if (part.second.get<string>("type") == "Equipment") m_parts.emplace_back(m_equipments.emplace_back(make_shared<EquipmentConfig>()))->Load(part.second);
		if (part.second.get<string>("type") == "Module") m_parts.emplace_back(m_modules.emplace_back(make_shared<ModuleConfig>()))->Load(part.second);
	}

	Model::SetFromJSON(pt);
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

		// �ʒu����

	}
}
