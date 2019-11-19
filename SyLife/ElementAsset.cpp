#include "ElementAsset.h"
#include "ElementEditor.h"

void ElementAsset::makeViewer()
{
	//g_viewerManagerPtr->makeViewer<ElementEditor>()->setPartAsset(shared_from_this());
}

void ElementAsset::load(const JSONValue& json)
{
	Asset::load(json);

	// mass
	m_mass = json[U"Mass"].get<double>();

	// color
	m_color = json[U"color"].get<Color>();

	// radius
	m_radius = sqrt(m_mass);

	// material
	m_material.load(json[U"material"]);
}

void ElementAsset::save(ptree& pt) const
{
	Asset::save(pt);

	// mass
	//pt.put<double>(U"mass", m_mass);

	// color
	//pt.put<String>(U"color", Unicode::Narrow(Format(m_color)));

	// material
	/*{
		ptree material;

		m_material.save(material);

		pt.add_child(U"material", material);
	}*/

	//pt.put(U"type", "ElementAsset");
}
