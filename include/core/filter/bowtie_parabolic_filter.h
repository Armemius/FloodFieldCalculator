#pragma once

#include "config.h"
#include "filter.h"

namespace pwn::ffc::core {
  class BowtieParabolicFilter final : public Filter {
    double m_distance;
    double m_min_thickness;
    double m_max_thickness;
    double m_radius;

  public:
    explicit BowtieParabolicFilter(const config::Filter &filter);

    BowtieParabolicFilter(const std::string &material, const std::optional<std::string> &id, double distance,
                          double min_thickness, double max_thickness, double radius, double rotation);

    [[nodiscard]] double calculateIntersectionDistance(const geometry::Ray &ray_transformed) const override;

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
