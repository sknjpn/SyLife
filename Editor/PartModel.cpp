#include "PartModel.h"

s3d::RectF PartModel::GetApproximateRect() const
{
	s3d::RectF ar(m_shapes.front().m_verticles.front(), m_shapes.front().m_verticles.front());

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_verticles)
		{
			if (ar.x > v.x) { ar.w += ar.x - v.x; ar.x = v.x; }
			if (ar.y > v.y) { ar.h += ar.y - v.y; ar.y = v.y; }
			if (ar.br().x < v.x) ar.w = v.x - ar.x;
			if (ar.br().y < v.y) ar.h = v.y - ar.y;
		}
	}

	return ar;
}

ptree PartModel::AddToJSON(ptree pt) const
{
	// mass
	pt.put<int>("mass", m_mass);

	// shapes
	{
		ptree shapes;

		for (const auto& v : m_shapes)
			shapes.push_back(std::make_pair("", v.AddToJSON(ptree())));

		pt.add_child("shapes", shapes);
	}

	return Model::AddToJSON(pt);
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