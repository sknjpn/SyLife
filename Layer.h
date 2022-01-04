#pragma once

#include "Object.h"

struct Layer : public Object {
  Polygon m_polygon;
  Color m_color;

  void load(const JSON &json) override;
  void save(JSON &json) const override;
};
