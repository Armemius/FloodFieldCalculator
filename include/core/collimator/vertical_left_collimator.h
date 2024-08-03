#pragma once
#include "collimator.h"

namespace pwn::ffc::core {
  class VerticalLeftCollimator : public Collimator {
  public:
    explicit VerticalLeftCollimator(const config::Collimator &collimator);

    VerticalLeftCollimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
