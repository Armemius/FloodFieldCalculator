#pragma once

#include "config.h"
#include "filter.h"

namespace pwn::ffc::core {
  class SlabFilter final : public Filter {
    double m_thickness;
    double m_distance;

  public:
    explicit SlabFilter(const config::Filter &filter);

    SlabFilter(const std::string &material, const std::optional<std::string> &id, double distance, double thickness,
               double rotation);

    [[nodiscard]] double calculateIntersectionDistance(const geometry::Ray &ray) const override;

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
