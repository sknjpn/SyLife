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

void ElementAsset::save(JSONWriter& json) const
{
	Asset::save(json);

	// mass
	json.key(U"mass").write(m_mass);

	// color
	json.key(U"color").write(m_color);

	// material
	{
		json.key(U"material").startObject();

		m_material.save(json);
		
		json.endObject();
	}

	// type
	json.key(U"type").write(U"ElementAsset");
}
