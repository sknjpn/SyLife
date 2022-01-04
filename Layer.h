#pragma once

#include "Model.h"

struct Layer : public Model {
  Polygon m_polygon;
  Color m_color;

  void load(const JSON &json) override;
  void save(JSON &json) const override;
};
