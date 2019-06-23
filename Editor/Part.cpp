#include "Part.h"

#include "ViewerManager.h"
#include "AssetManager.h"

void PartModel::MakeViewers()
{
	g_viewerManagerPtr->AddViewer<PartViewer>(dynamic_pointer_cast<PartModel>(shared_from_this()));
}

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

double PartModel::GetRectInertia() const
{
	double w = GetApproximateRect().w;
	double h = GetApproximateRect().h;

	return  m_mass * (w * w + h * h) / 12.0;
}

s3d::Vec2 PartModel::GetCenter() const
{
	return GetApproximateRect().center();
}

ptree PartModel::AddToJSON(ptree pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// shapes
	{
		ptree shapes;

		for (const auto& v : m_shapes)
			shapes.push_back(std::make_pair("", v.Save()));

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

ptree PartConfig::AddToJSON(ptree pt) const
{
	// model
	pt.put("name", m_model->m_name);

	// position
	{
		ptree position;

		position.put("x", m_position.x);
		position.put("y", m_position.y);

		pt.push_back(std::make_pair("position", position));
	}

	// rotation
	pt.put("rotation", m_rotation);

	return Model::AddToJSON(pt);
}

void PartConfig::SetFromJSON(const ptree & pt)
{
	// model
	m_model = g_assetManagerPtr->GetModel<PartModel>(pt.get<string>("name"));

	// position
	m_position = s3d::Vec2(pt.get<double>("position.x"), pt.get<double>("position.y"));

	// rotation
	m_rotation = pt.get<double>("rotation");

	Model::SetFromJSON(pt);
}

void PartViewer::Update()
{

}
