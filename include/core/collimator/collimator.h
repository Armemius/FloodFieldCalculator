#pragma once

#include "config.h"
#include "geometry/intersectable.h"

namespace pwn::ffc::core {
  class Collimator : public geometry::Intersectable {
  protected:
    double m_distance;
    double m_shift;

  public:
    explicit Collimator(const config::Collimator &collimator);

    Collimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override = 0;
  };
}
