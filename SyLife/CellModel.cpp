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


	CalculateDisk();

	Model::SetFromJSON(pt);
}


void CellModel::CalculateDisk()
{
	// mass
	{
		m_mass = 0.0;

		for (const auto& p : m_parts) m_mass += p->m_model->m_mass;
	}

	// center
	{
		// body
		Vector2D center(0.0, 0.0);

		for (const auto& p : m_parts) center += p->m_model->m_mass * (p->m_position + ((p->m_model->m_approximateRect.first + p->m_model->m_approximateRect.second) / 2.0).rotated(p->m_rotation));

		center /= m_mass;

		// ˆÊ’u’²®
		for (const auto& p : m_parts) p->m_position -= center;
	}

	// inertia
	{
		m_inertia = 0.0;

		for (const auto& p : m_parts) m_inertia += p->GetInertia();
	}

	// radius
	m_radius = sqrt(2 * m_inertia / m_mass);
}
