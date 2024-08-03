#pragma once
#include "collimator.h"

namespace pwn::ffc::core {
  class VerticalRightCollimator : public Collimator {
  public:
    explicit VerticalRightCollimator(const config::Collimator &collimator);

    VerticalRightCollimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
