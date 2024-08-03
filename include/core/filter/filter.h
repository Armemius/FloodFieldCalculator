#pragma once
#include <string>

#include "geometry/intersectable.h"

namespace pwn::ffc::core {
  class Filter : public geometry::Intersectable {
    std::string m_id;
    std::string m_material;

  public:
    explicit Filter(std::string material, std::string id = "UNKNOWN");

    [[nodiscard]] virtual double calculateIntersectionDistance(const geometry::Ray &ray) const = 0;

    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override = 0;

    [[nodiscard]] const std::string &getMaterial() const;

    [[nodiscard]] const std::string &getId() const;
  };
}
