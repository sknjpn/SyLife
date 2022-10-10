﻿#include "Shape.h"

void Shape::preRender() {
  Image image((m_polygon.boundingRect().size * GeneralSetting::GetInstance().m_textureScale).asPoint());

  for (const auto& layer : *this) {
    layer.m_polygon.movedBy(-m_polygon.boundingRect().pos)
        .scaled(GeneralSetting::GetInstance().m_textureScale)
        .overwrite(image, ColorF(layer.m_color, 1.0));
  }

  m_preRenderTexture = Texture(image);
}

bool Shape::updateProperties() {
  if (isEmpty()) {
    m_polygon = Polygon();

    return false;
  }

  // 初期化
  m_polygon = front().m_polygon;

  Array<Polygon> copies;
  for (const auto& l : *this) {
    if (l.m_polygon.isEmpty())
      return false;

    copies.emplace_back(l.m_polygon);
  }

  Polygon result;
  while (!copies.empty()) {
    bool flag = true;

    for (auto it = copies.begin(); it != copies.end(); ++it) {
      auto polygons = Geometry2D::Or(result, *it);

      if (polygons.size() == 1) {
        result = polygons.front();
        copies.erase(it);

        flag = false;

        break;
      }
    }

    if (flag)
      return false;
  }

  m_polygon = result;

  preRender();

  return true;
}

double Shape::getInertia(double mass) const {
  const auto centroid = m_polygon.centroid();
  const auto numTriangles = m_polygon.num_triangles();
  const auto area = m_polygon.area();

  double inertia = 0;
  for (int i = 0; i < numTriangles; ++i) {
    const auto triangle = m_polygon.triangle(i);
    const auto v0 = triangle.p0 - triangle.centroid();
    const auto v1 = triangle.p1 - triangle.centroid();
    const auto v2 = triangle.p2 - triangle.centroid();
    const auto m = mass * triangle.area() / area;
    const auto it = m / 12 * (v0.lengthSq() + v1.lengthSq() + v2.lengthSq());
    const auto id = m * (triangle.centroid() - centroid).lengthSq();

    inertia += it + id;
  }

  return inertia;
}

RectF Shape::getTileSize() const {
  if (m_polygon.isEmpty())
    return RectF();

  RectF result(m_polygon.vertices().front().x, m_polygon.vertices().front().y, 0, 0);
  for (const auto& v : m_polygon.vertices()) {
    if (v.x < result.x)
      result.x = v.x;
    if (v.y < result.y)
      result.y = v.y;
  }
  for (const auto& v : m_polygon.vertices()) {
    if (result.br().x < v.x)
      result.w = v.x - result.x;
    if (result.br().y < v.y)
      result.h = v.y - result.y;
  }

  return result;
}

void Shape::load(const JSON& json) {
  Object::load(json);

  // layers
  for (auto layer : json[U"layers"].arrayView())
    emplace_back().load(layer);

  updateProperties();
}

void Shape::save(JSON& json) const {
  Object::save(json);

  // layers
  {
    Array<JSON> jsonArrayLayers;

    for (const auto& layer : *this)
      layer.save(jsonArrayLayers.emplace_back());

    json[U"layers"] = jsonArrayLayers;
  }
}
