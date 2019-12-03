#include "PartAsset.h"

#include "PartState.h"
#include "PartShapeViewer.h"

void PartAsset::drawIcon() const
{
	auto rect = m_shape.getPolygon().boundingRect();
	rect = RectF(1.0, 1.0).scaled(Max(rect.size.x, rect.size.y)).setCenter(rect.center());

	const auto t = Transformer2D(Mat3x2::Translate(-rect.pos).scaled(1.0 / rect.size.x));

	m_shape.draw(0.5);
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
