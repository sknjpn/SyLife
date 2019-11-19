#include "Layer.h"

RectF Layer::getRect() const
{
	if (m_polygon.isEmpty()) return RectF();

	RectF result(m_polygon.vertices().front().x, m_polygon.vertices().front().y, 0, 0);
	for (const auto& v : m_polygon.vertices())
	{
		if (v.x < result.x) result.x = v.x;
		if (v.y < result.y) result.y = v.y;
	}
	for (const auto& v : m_polygon.vertices())
	{
		if (result.br().x < v.x) result.w = v.x - result.x;
		if (result.br().y < v.y) result.h = v.y - result.y;
	}

	return result;
}

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
