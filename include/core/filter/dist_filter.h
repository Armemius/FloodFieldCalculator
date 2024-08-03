#pragma once

#include "config.h"
#include "filter.h"

namespace pwn::ffc::core {
  class DistFilter final : public Filter {
    double m_distance;
    double m_radius;

  public:
    explicit DistFilter(const config::Filter &filter);

    DistFilter(const std::string &material, const std::string &id, double distance, double radius);

    [[nodiscard]] double calculateIntersectionDistance(const geometry::Ray &ray) const override;

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
