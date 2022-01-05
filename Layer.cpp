#include "Layer.h"

void Layer::load(const JSON& json) {
  Object::load(json);

  // color
  m_color = json[U"color"].get<Color>();

  // polygon
  {
    Array<Vec2>        outer;
    Array<Array<Vec2>> holes;

    for (const auto& v : json[U"polygon"][U"outer"].arrayView())
      outer.emplace_back(v.get<Vec2>());

    if (json[U"polygon"][U"holes"].isArray())
      for (const auto& hole : json[U"polygon"][U"holes"].arrayView())
        for (const auto& v : hole.arrayView())
          outer.emplace_back(v.get<Vec2>());

    m_polygon = Polygon(outer, holes);
  }
}

void Layer::save(JSON& json) const {
  Object::save(json);

  // color
  json[U"color"] = m_color;

  // polygon
  JSON jsonPolygon;
  {
    jsonPolygon[U"outer"] = m_polygon.outer();

    // holes
    {
      Array<JSON> jsonArrayHoles;

      for (Array<Vec2> hole : m_polygon.inners())
        jsonArrayHoles.emplace_back(hole);

      jsonPolygon[U"holes"] = jsonArrayHoles;
    }
  }
  json[U"polygon"] = jsonPolygon;
}
