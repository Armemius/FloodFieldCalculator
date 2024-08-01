#pragma once

#include "ray.h"

namespace pwn::ffc::geometry {
  class Intersectable {
  public:
    virtual ~Intersectable() = default;

    [[nodiscard]] virtual bool doesIntersect(const Ray& ray) const = 0;
  };
}
