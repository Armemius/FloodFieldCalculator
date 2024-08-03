#pragma once

#include "config.h"
#include "filter.h"

namespace pwn::ffc::core {
  class BowtieCylindricalFilter final : public Filter {
    double m_thickness;
    double m_distance;
    double m_radius;

  public:
    explicit BowtieCylindricalFilter(const config::Filter &filter);

    BowtieCylindricalFilter(const std::string &material, const std::string &id, double distance, double thickness,
                            double radius);

    [[nodiscard]] double calculateIntersectionDistance(const geometry::Ray &ray) const override;

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
