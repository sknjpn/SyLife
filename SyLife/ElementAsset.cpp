#include "ElementAsset.h"

#include "ViewerManager.h"

#include "ElementEditor.h"

void ElementAsset::makeViewer()
{
	g_viewerManagerPtr->makeViewer<ElementEditor>()->setPartAsset(shared_from_this());
}

void ElementAsset::load(const ptree& pt)
{
	Asset::load(pt);

	// mass
	m_mass = pt.get<double>("mass");

	// color
	m_color = Parse<Color>(Unicode::Widen(pt.get<string>("color")));

	// radius
	m_radius = sqrt(m_mass);

	// material
	m_material.load(pt.get_child("material"));
}

void ElementAsset::save(ptree& pt) const
{
	Asset::save(pt);

	// mass
	pt.put<double>("mass", m_mass);

	pt.put<string>("color", Unicode::Narrow(Format(m_color)));

	// material
	{
		ptree material;

		m_material.save(material);

		pt.add_child("material", material);
	}

	pt.put("type", "ElementAsset");
}
