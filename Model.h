#pragma once

#include "common.h"

class Model : public std::enable_shared_from_this<Model> {
public:
  virtual ~Model() = default;

  // JSON
  virtual void load(const JSON &json) {}
  virtual void save(JSON &json) const {}
};
