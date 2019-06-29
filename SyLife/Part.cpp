#include "Part.h"
#include "AssetManager.h"

Vector2D PartModel::GetApproximateRectTopLeft() const
{
	double x = m_shapes.front().m_polygon.vertices().front().x;
	double y = m_shapes.front().m_polygon.vertices().front().y;

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_polygon.vertices())
		{
			if (x > v.x) x = v.x;
			if (y > v.y) y = v.y;
		}
	}

	return Vector2D(x, y);
}

Vector2D PartModel::GetApproximateRectBottomDown() const
{
	double x = m_shapes.front().m_polygon.vertices().front().x;
	double y = m_shapes.front().m_polygon.vertices().front().y;

	for (const auto& s : m_shapes)
	{
		for (const auto& v : s.m_polygon.vertices())
		{
			if (x < v.x) x = v.x;
			if (y < v.y) y = v.y;
		}
	}

	return Vector2D(x, y);
}

double PartModel::GetRectInertia() const
{
	auto w = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).m_x;
	auto h = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).m_y;

	return  m_mass * (w * w + h * h) / 12.0;
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
