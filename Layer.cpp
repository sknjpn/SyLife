#include "Layer.h"

void Layer::load(const JSONValue& json)
{
	Model::load(json);

	// color
	m_color = json[U"color"].get<Color>();

	// polygon
	{
		Array<Vec2> verticles;

		for (const auto& v : json[U"verticles"].arrayView())
			verticles.emplace_back(v.get<Vec2>());

		m_polygon = Polygon(verticles);
	}
}

void Layer::save(JSONWriter& json) const
{
	Model::save(json);

	// color
	json.key(U"color").write(m_color);

	// verticles
	json.key(U"vertices").writeArray(m_polygon.vertices());
}
