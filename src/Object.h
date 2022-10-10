#pragma once

#include "common.h"

class Object : public std::enable_shared_from_this<Object>
{
public:
  virtual ~Object() = default;

  // JSON
  virtual void load(const JSON& json) { }
  virtual void save(JSON& json) const { }
};
