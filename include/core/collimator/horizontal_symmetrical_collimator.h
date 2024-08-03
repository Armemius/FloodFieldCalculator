#pragma once
#include "collimator.h"
#include "vertical_left_collimator.h"
#include "vertical_right_collimator.h"

namespace pwn::ffc::core {
  class HorizontalSymmetricalCollimator final : public Collimator {
  public:
    explicit HorizontalSymmetricalCollimator(const config::Collimator &collimator);

    HorizontalSymmetricalCollimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
