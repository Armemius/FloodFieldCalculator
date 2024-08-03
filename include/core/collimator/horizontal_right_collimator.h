#pragma once
#include "collimator.h"

namespace pwn::ffc::core {
  class HorizontalBottomCollimator : public Collimator {
  public:
    explicit HorizontalBottomCollimator(const config::Collimator &collimator);

    HorizontalBottomCollimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
