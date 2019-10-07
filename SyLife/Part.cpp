#include "Part.h"

#include "AssetManager.h"
#include "ViewerManager.h"

void PartModel::MakeViewer()
{
	g_viewerManagerPtr->MakeViewer<PartViewer>()->SetModel(shared_from_this());
	g_viewerManagerPtr->MakeViewer<PartShapeViewer>()->SetModel(shared_from_this());
}

shared_ptr<PartState> PartModel::MakeState()
{
	return make_shared<PartState>();
}

RectF PartModel::GetApproximateRect() const
{
	const Vec2 tl = GetApproximateRectTopLeft();
	const Vec2 br = GetApproximateRectBottomDown();

	return RectF(tl, br - tl);
}

Vec2 PartModel::GetApproximateRectTopLeft() const
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

	return Vec2(x, y);
}

Vec2 PartModel::GetApproximateRectBottomDown() const
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

	return Vec2(x, y);
}

double PartModel::GetRectInertia() const
{
	auto w = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).x;
	auto h = (GetApproximateRectBottomDown() - GetApproximateRectTopLeft()).y;

	return  m_mass * (w * w + h * h) / 12.0;
}

void PartModel::load_this(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// shapes
	for (auto shape : pt.get_child("shapes"))
		m_shapes.emplace_back().load_this(shape.second);

	// material
	m_material.load(pt.get_child("material"));

	Model::load_this(pt);
}

void PartModel::save_this(ptree& pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// shapes
	{
		ptree shapes;

		for (const auto& v : m_shapes)
		{
			ptree shape; v.save(shape);
			shapes.push_back(std::make_pair("", shape));
		}

		pt.add_child("shapes", shapes);
	}

	// material
	{
		ptree material;

		m_material.save(material);

		pt.add_child("material", material);
	}

	Model::save_this(pt);

	pt.put("type", "PartModel");
}

void PartConfig::load_this(const ptree& pt)
{
	// model
	m_partModel = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("model"));

	// position
	m_position = Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::load_this(pt);
}

void PartConfig::save_this(ptree& pt) const
{
	// model
	pt.put("model", m_partModel->getName());

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	Model::save_this(pt);

	pt.put("type", "PartConfig");
}
