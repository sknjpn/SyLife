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

void Layer::load(const ptree& pt)
{
	Model::load(pt);

	// color
	m_color = Parse<Color>(Unicode::Widen(pt.get<string>("color")));

	// polygon
	{
		Array<Vec2> verticles;

		for (auto v : pt.get_child("verticles"))
			verticles.emplace_back(v.second.get<double>("x"), v.second.get<double>("y"));

		m_polygon = Polygon(verticles);
	}
}

void Layer::save(ptree& pt) const
{
	Model::save(pt);

	// color
	pt.put<string>("color", Unicode::Narrow(Format(m_color)));

	// verticles
	{
		ptree verticles;

		{
			ptree verticle;

			for (const auto& v : m_polygon.vertices())
			{
				verticle.put("x", v.x);
				verticle.put("y", v.y);

				verticles.push_back(std::make_pair("", verticle));
			}
		}

		pt.add_child("verticles", verticles);
	}
}
