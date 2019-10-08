#include "ElementAsset.h"

void ElementAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<ElementEditor>()->setModel(shared_from_this());
}

void ElementAsset::load_this(const ptree& pt)
{
	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = Color(pt.get<int>("color.r"), pt.get<int>("color.g"), pt.get<int>("color.b"));

	// radius
	m_radius = sqrt(m_mass);

	// material
	m_material.load(pt.get_child("material"));

	Model::load_this(pt);
}

void ElementAsset::save_this(ptree& pt) const
{
	// mass
	pt.put<double>("mass", m_mass);

	// color
	{
		ptree color;

		color.put("r", m_color.r);
		color.put("g", m_color.g);
		color.put("b", m_color.b);

		pt.add_child("color", color);
	}

	// material
	{
		ptree material;

		m_material.save(material);

		pt.add_child("material", material);
	}

	Model::save_this(pt);

	pt.put("type", "ElementAsset");
}
