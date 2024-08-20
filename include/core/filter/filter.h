#pragma once

#include <optional>
#include <string>

#include "geometry/intersectable.h"

namespace pwn::ffc::core {
  /// Class representing volumetric filter
  class Filter : public geometry::Intersectable {
    std::optional<std::string> m_id;
    std::string m_material;

  protected:
    double m_rotation;

  public:
    explicit Filter(std::string material, const std::optional<std::string> &id, double rotation);

    /**
     * Calculates distance traveled by the ray inside the object
     *
     * @param ray ray that intersects filter
     * @return distance traveled by the ray inside the object
     */
    [[nodiscard]] virtual double calculateIntersectionDistance(const geometry::Ray &ray) const = 0;

    /**
     * Checks if given ray intersects object
     *
     * @param ray ray that can intersect object
     * @return true if ray goes through object, false otherwise
     */
    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override = 0;

    /**
     * Returns filter material id
     *
     * @return filter material id
     */
    [[nodiscard]] const std::string &getMaterial() const;

    /**
     * Returns filter id
     *
     * @return filter id
     */
    [[nodiscard]] const std::optional<std::string> &getId() const;
  };
}
