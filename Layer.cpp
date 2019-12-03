#include "Layer.h"

void Layer::load(const JSONValue& json)
{
	Model::load(json);

	// color
	m_color = json[U"color"].get<Color>();

	// polygon
	{
		Array<Vec2> outer;
		Array<Array<Vec2>> holes;

		for (const auto& v : json[U"polygon"][U"outer"].arrayView())
			outer.emplace_back(v.get<Vec2>());

		for (const auto& hole : json[U"polygon"][U"holes"].arrayView())
			for (const auto& v : hole.arrayView())
				outer.emplace_back(v.get<Vec2>());

		m_polygon = Polygon(outer, holes);
	}
}

void Layer::save(JSONWriter& json) const
{
	Model::save(json);

	// color
	json.key(U"color").write(m_color);

	// polygon
	json.key(U"polygon").startObject();
	{
		// outer
		json.key(U"outer").writeArray(m_polygon.outer());

		// holes
		json.key(U"holes").startArray();
		for(auto hole : m_polygon.inners())
		{
			json.writeArray(hole);
		}
		json.endArray();
	}
	json.endObject();
}
