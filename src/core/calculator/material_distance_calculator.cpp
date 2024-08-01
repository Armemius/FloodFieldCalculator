#include <utility>

#include "core/calculator/material_distance_calculator.h"

namespace pwn::ffc::core {
  MaterialDistanceCalculator::MaterialDistanceCalculator(std::string accepted_material)
    : DistanceCalculator(),
      m_accepted_material(std::move(accepted_material)) {
  }

  Calculator *MaterialDistanceCalculator::addFilter(std::unique_ptr<Filter> filter) {
    if (filter->getMaterial() == this->m_accepted_material) {
      this->m_filters.push_back(std::move(filter));
    }
    return this;
  }
}
