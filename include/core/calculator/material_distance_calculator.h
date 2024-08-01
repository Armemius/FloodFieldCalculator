#pragma once

#include "distance_calculator.h"

namespace pwn::ffc::core {
  class MaterialDistanceCalculator final : public DistanceCalculator {
    std::string m_accepted_material;
  public:
    explicit MaterialDistanceCalculator(std::string accepted_material);
    Calculator *addFilter(std::unique_ptr<Filter> filter) override;
  };
}
