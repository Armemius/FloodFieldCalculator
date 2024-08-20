#pragma once

#include "config.h"
#include "filter.h"

#define FIRST_ITERATION_VALUE (-1)

namespace pwn::ffc::core {
  class BowtieGaussFilter final : public Filter {
    double m_thickness;
    double m_distance;
    double m_sigma;
    double m_depth;

    double function(double x, double k) const;

    double derivative(double x, double k) const;

    double solveEquation(double k, double prev_value = FIRST_ITERATION_VALUE, double eps = 0.0001) const;

  public:
    explicit BowtieGaussFilter(const config::Filter &filter);

    BowtieGaussFilter(const std::string &material, const std::optional<std::string> &id, double distance,
                      double thickness,
                      double sigma, double depth, double rotation);

    [[nodiscard]] double calculateIntersectionDistance(const geometry::Ray &ray_transformed) const override;

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override;
  };
}
