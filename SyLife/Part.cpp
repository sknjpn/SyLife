#include "Part.h"
#include "AssetManager.h"

s3d::RectF PartModel::GetApproximateRect() const
{
	double tlx = m_shapes.front().m_polygon.vertices().front().x;
	double tly = m_shapes.front().m_polygon.vertices().front().y;
	double brx = m_shapes.front().m_polygon.vertices().front().x;
	double bry = m_shapes.front().m_polygon.vertices().front().y;

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_polygon.vertices())
		{
			if (tlx > v.x) tlx = v.x;
			if (tly > v.y) tly = v.y;
			if (brx < v.x) brx = v.x;
			if (bry < v.y) bry = v.y;
		}
	}

	return s3d::RectF(tlx, tly, brx - tlx, bry - tly);
}

double PartModel::GetRectInertia() const
{
	return  m_mass * (pow(GetApproximateRect().w, 2) + pow(GetApproximateRect().h, 2)) / 12.0;
}

void PartModel::SetFromJSON(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().SetFromJSON(shape.second);

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
