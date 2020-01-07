#include "ProteinAsset.h"

void ProteinAsset::drawIcon() const
{
	static Texture particle(U"resources/image/particle.png", TextureDesc::Mipped);

	particle.resized(1.0, 1.0).drawAt(0.5, 0.5, m_color);
}

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
