#pragma once

#include "Asset_Editor.h"

class ProteinAsset;

class ProteinAsset_Editor : public Asset_Editor {
public:
  ProteinAsset_Editor(const std::shared_ptr<Asset>& asset)
      : Asset_Editor(asset) { }

  void init() override;
  void update() override;
};
