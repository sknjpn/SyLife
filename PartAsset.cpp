#include "PartAsset.h"
#include "PartConfig.h"
#include "PartState.h"

void PartAsset::preRender(Image& image, const std::shared_ptr<PartConfig>& partConfig) const
{
	for (const auto& layer : m_shape)
		layer.m_polygon
		.rotated(partConfig->getRotation())
		.movedBy(partConfig->getPosition())
		.scaled(GeneralSetting::GetInstance().m_textureScale)
		.movedBy(Vec2(image.size()) / 2.0)
		.overwrite(image, ColorF(layer.m_color, 1.0));
}

void PartAsset::load(const JSONValue& json)
{
	Asset::load(json);

	// mass
	m_mass = json[U"mass"].get<double>();

	// layers
	m_shape.load(json);

	// material
	m_material.load(json[U"material"]);
}

void PartAsset::save(JSONWriter& json) const
{
	Asset::save(json);

	// mass
	json.key(U"mass").write(m_mass);

	// layers
	m_shape.save(json);

	// material
	{
		json.key(U"material").startObject();

		m_material.save(json);

		json.endObject();
	}
}
