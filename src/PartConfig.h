﻿#pragma once

#include "Object.h"

class PartAsset;

class PartConfig : public Object {
  std::shared_ptr<PartAsset> m_partAsset;

  Vec2   m_position;
  double m_rotation;

public:
  // Get
  const std::shared_ptr<PartAsset>& getPartAsset() const { return m_partAsset; }

  const Vec2& getPosition() const { return m_position; }
  double      getRotation() const { return m_rotation; }

  Mat3x2 getMat3x2() const {
    return Mat3x2::Rotate(getRotation()).translated(getPosition());
  }

  // Set
  void setPartAsset(const std::shared_ptr<PartAsset>& partAsset) { m_partAsset = partAsset; }
  void setPosition(const Vec2& position) { m_position = position; }
  void setRotation(double rotation) { m_rotation = rotation; }

  Vec2 getCentroid() const;

  void load(const JSON& json) override;
  void save(JSON& json) const override;
};
