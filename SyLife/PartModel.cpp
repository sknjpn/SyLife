#include "PartModel.h"

double PartModel::GetRectInertia() const
{
	double w = m_approximateRect.second.m_x - m_approximateRect.first.m_x;
	double h = m_approximateRect.second.m_y - m_approximateRect.first.m_y;

	return  m_mass * (w * w + h * h) / 12.0;
}

void PartModel::SetFromJSON(const ptree & pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().SetFromJSON(shape.second);

	Model::SetFromJSON(pt);
}