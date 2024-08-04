#pragma once

#include "ray.h"

namespace pwn::ffc::geometry {
  /// Class that represents volumetric object that can be intersected by ray
  class Intersectable {
  public:
    virtual ~Intersectable() = default;

    /**
     * Checks if given ray intersects object
     *
     * @param ray ray that can intersect object
     * @return true if ray goes through object, false otherwise
     */
    [[nodiscard]] virtual bool doesIntersect(const Ray &ray) const = 0;
  };
}
