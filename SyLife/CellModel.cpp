#include "CellModel.h"

void CellModel::SetFromJSON(const ptree & pt)
{
	// parts
	for (auto part : pt.get_child("parts")) AddPartConfig(part.second);

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
