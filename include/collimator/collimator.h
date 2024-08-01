#pragma once
#include "geometry/intersectable.h"

namespace pwn::ffc::core {
  class Collimator : public geometry::Intersectable {
  public:
    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override = 0;
  };
}
