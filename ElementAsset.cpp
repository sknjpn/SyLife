#include "ElementAsset.h"

void ElementAsset::drawIcon() const
{
	static Texture particle(U"assets/image/particle.png", TextureDesc::Mipped);

	particle.resized(1.0, 1.0).drawAt(0.5, 0.5, m_color);
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
