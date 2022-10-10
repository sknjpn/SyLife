#pragma once

#include "PartAsset.h"

class Part_BodyAsset : public PartAsset
{
public:
  Part_BodyAsset()
    : m_image(800, 800)
  {
  }

  Texture m_texture;
  Image   m_image;

  // State
  std::shared_ptr<PartState> makePartState(const std::shared_ptr<PartConfig>& partConfig) const override;

  // JSON
  void load(const JSON& json) override;
  void save(JSON& json) const override;

  bool isPreRenderOnStateEnabled() const override { return true; }
  bool isDrawOnStateEnabled() const override { return false; }

  void draw(double a) override;
  void preRender(Image& image,
      const std::shared_ptr<PartConfig>& partConfig) const override;

  // TypeName
  String getTypeName() override { return U"Part_BodyAsset"; }
};
