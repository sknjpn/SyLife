#pragma once

#include "Asset.h"

#include "Shape.h"
#include "Storage.h"

class PartState;
class PartConfig;

class PartAsset : public Asset {
protected:
  double m_mass;
  Storage m_material;
  Shape m_shape;

public:
  // Get
  double getMass() const { return m_mass; }
  const Storage &getMaterial() const { return m_material; }
  const Shape &getShape() const { return m_shape; }
  Storage &getMaterial() { return m_material; }
  Shape &getShape() { return m_shape; }

  // Set
  void setMass(double mass) { m_mass = mass; }

  // State
  virtual std::shared_ptr<PartState>
  makePartState(const std::shared_ptr<PartConfig> &partConfig) const = 0;

  // 描画処理を行うかどうか
  virtual bool isPreRenderOnStateEnabled() const { return true; }
  virtual bool isDrawOnStateEnabled() const { return false; }

  // Draw
  virtual void draw(double a = 0.5) { m_shape.draw(a); }

  virtual void preRender(Image &image,
                         const std::shared_ptr<PartConfig> &partConfig) const;

  // JSON
  void load(const JSON &json) override;
  void save(JSON &json) const override;

  // TypeName
  String getTypeName() override { return U"PartAsset"; }
};
