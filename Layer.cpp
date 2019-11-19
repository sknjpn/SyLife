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

void Layer::save(const JSONWriter& json) const
{
	Model::save(json);

	// color
	//pt.put<String>(U"color", Unicode::Narrow(Format(m_color)));

	// verticles
	/*{
		ptree verticles;

		{
			ptree verticle;

			for (const auto& v : m_polygon.vertices())
			{
				verticle.put(U"x", v.x);
				verticle.put(U"y", v.y);

				verticles.push_back(std::make_pair(U"", verticle));
			}
		}

		pt.add_child(U"verticles", verticles);
	}*/
}
