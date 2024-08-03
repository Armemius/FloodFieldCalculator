#pragma once
#include "collimator.h"

namespace pwn::ffc::core {
  class HorizontalTopCollimator : public Collimator {
  public:
    explicit HorizontalTopCollimator(const config::Collimator &collimator);

    HorizontalTopCollimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
