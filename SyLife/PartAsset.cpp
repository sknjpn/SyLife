#include "PartAsset.h"

#include "AssetManager.h"
#include "ViewerManager.h"

#include "PartState.h"
#include "PartEditor.h"
#include "PartShapeViewer.h"

void PartAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<PartEditor>()->setModel(shared_from_this());
	g_viewerManagerPtr->makeViewer<PartShapeViewer>()->setModel(shared_from_this());
}

shared_ptr<PartState> PartAsset::makeState()
{
	return make_shared<PartState>();
}

RectF PartAsset::getApproximateRect() const
{
	const Vec2 tl = getApproximateRectTopLeft();
	const Vec2 br = getApproximateRectBottomDown();

	return RectF(tl, br - tl);
}

Vec2 PartAsset::getApproximateRectTopLeft() const
{
	double x = m_shape.front().m_polygon.vertices().front().x;
	double y = m_shape.front().m_polygon.vertices().front().y;

	for (const auto& l : m_shape)
	{
		for (const auto& v : l.m_polygon.vertices())
		{
			if (x > v.x) x = v.x;
			if (y > v.y) y = v.y;
		}
	}

	return Vec2(x, y);
}

Vec2 PartAsset::getApproximateRectBottomDown() const
{
	double x = m_shape.front().m_polygon.vertices().front().x;
	double y = m_shape.front().m_polygon.vertices().front().y;

	for (const auto& l : m_shape)
	{
		for (const auto& v : l.m_polygon.vertices())
		{
			if (x < v.x) x = v.x;
			if (y < v.y) y = v.y;
		}
	}

	return Vec2(x, y);
}

double PartAsset::getRectInertia() const
{
	auto w = (getApproximateRectBottomDown() - getApproximateRectTopLeft()).x;
	auto h = (getApproximateRectBottomDown() - getApproximateRectTopLeft()).y;

	return  m_mass * (w * w + h * h) / 12.0;
}

void PartAsset::load_this(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// layers
	m_shape.load(pt);

	// material
	m_material.load(pt.get_child("material"));

	Model::load_this(pt);
}

void PartAsset::save_this(ptree& pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// layers
	m_shape.save(pt);

	// material
	{
		ptree material;

		m_material.save(material);

		pt.add_child("material", material);
	}

	Model::save_this(pt);

	pt.put("type", "PartAsset");
}
