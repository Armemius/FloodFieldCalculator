#pragma once
#include "collimator.h"
#include "vertical_left_collimator.h"
#include "vertical_right_collimator.h"

namespace pwn::ffc::core {
  class VerticalSymmetricalCollimator final : public Collimator {
  public:
    explicit VerticalSymmetricalCollimator(const config::Collimator &collimator);

    VerticalSymmetricalCollimator(double distance, double shift);

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
