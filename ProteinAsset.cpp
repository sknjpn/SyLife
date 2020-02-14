#include "ProteinAsset.h"
#include "ProteinAsset_Editor.h"

void ProteinAsset::load(const JSONValue& json)
{
	Asset::load(json);

	// radius
	m_radius = json[U"radius"].get<double>();

	// color
	m_color = json[U"color"].get<Color>();

	// material
	m_material.load(json[U"material"]);
}

void ProteinAsset::save(JSONWriter& json) const
{
	Asset::save(json);

	// radius
	json.key(U"radius").write(m_radius);

	// color
	json.key(U"color").write(m_color);

	// material
	{
		json.key(U"material").startObject();
		
		m_material.save(json);
		
		json.endObject();
	}
}

void ProteinAsset::makeEditor(const std::shared_ptr<EasyViewer>& parent) const
{
	parent->addChildViewer<ProteinAsset_Editor>();
}
