#include "Part.h"
#include "AssetManager.h"

s3d::RectF PartModel::GetApproximateRect() const
{
	return s3d::RectF(
		m_approximateRect.first.m_x,
		m_approximateRect.first.m_y,
		m_approximateRect.second.m_x - m_approximateRect.first.m_x,
		m_approximateRect.second.m_y - m_approximateRect.first.m_y);
}

double PartModel::GetRectInertia() const
{
	double w = m_approximateRect.second.m_x - m_approximateRect.first.m_x;
	double h = m_approximateRect.second.m_y - m_approximateRect.first.m_y;

	return  m_mass * (w * w + h * h) / 12.0;
}

void PartModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().SetFromJSON(shape.second);

	// approximateRect
	{
		auto& afx = m_approximateRect.first.m_x;
		auto& afy = m_approximateRect.first.m_y;
		auto& asx = m_approximateRect.second.m_x;
		auto& asy = m_approximateRect.second.m_y;

		afx = m_shapes.front().m_polygon.vertices().front().x;
		afy = m_shapes.front().m_polygon.vertices().front().y;
		asx = m_shapes.front().m_polygon.vertices().front().x;
		asy = m_shapes.front().m_polygon.vertices().front().y;

		for (const auto& s : m_shapes)
		{
			for (const auto& v : s.m_polygon.vertices())
			{
				if (afx > v.x) afx = v.x;
				if (afy > v.y) afy = v.y;
				if (asx < v.x) asx = v.x;
				if (asy < v.y) asy = v.y;
			}
		}
	}

	Model::SetFromJSON(pt);
}

void PartConfig::SetFromJSON(const ptree& pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("model"));

	// position
	m_position = Vector2D(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}
