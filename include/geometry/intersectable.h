#pragma once

#include "ray.h"

namespace pwn::ffc::geometry {
  class Intersectable {
  public:
    virtual ~Intersectable() = default;

    virtual bool doesIntersect(const Ray& ray) = 0;
  };
}
