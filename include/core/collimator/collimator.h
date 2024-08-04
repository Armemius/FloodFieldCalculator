#pragma once

#include "config.h"
#include "geometry/intersectable.h"

namespace pwn::ffc::core {
  /**
   * Class representing collimator
   *
   * Collimator unlike filter completely jams the signal passing through it
   */
  class Collimator : public geometry::Intersectable {
  protected:
    double m_distance;
    double m_shift;

  public:
    explicit Collimator(const config::Collimator &collimator);

    Collimator(double distance, double shift);

    /**
     * Checks if given ray intersects object
     *
     * @param ray ray that can intersect object
     * @return true if ray goes through object, false otherwise
     */
    [[nodiscard]] bool doesIntersect(const geometry::Ray &ray) const override = 0;
  };
}
