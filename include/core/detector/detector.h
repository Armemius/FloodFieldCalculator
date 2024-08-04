#pragma once

#include "geometry/plane.h"
#include "geometry/point.h"

namespace pwn::ffc::core {
  /// Class representing detector parameters and its geometry
  class Detector {
  protected:
    const geometry::Plane<int> m_resolution;
    const geometry::Plane<double> m_size;

    /**
     * Calculates geomateric point on detector plane that correcponds to a specified pixel
     *
     * @param x x pixel coordinate
     * @param y y pixel coordinate
     * @return geometric point on detector plane that reads specific pixel
     */
    [[nodiscard]] virtual geometry::Point calculatePixelCoords(int x, int y) const = 0;

  public:
    virtual ~Detector() = default;

    Detector(geometry::Plane<int> resolution, geometry::Plane<double> size);

    /**
     * @brief Returns detector resolution
     */
    [[nodiscard]] geometry::Plane<int> getResolution() const;

    /**
     * @brief Returns geometric detector size
     */
    [[nodiscard]] geometry::Plane<double> getSize() const;

    /**
     * Calculates geomateric point on detector plane that correcponds to a specified pixel
     *
     * This function also includes check for pixel bounds
     *
     * @param x x pixel coordinate
     * @param y y pixel coordinate
     * @return geometric point on detector plane that reads specific pixel
     * @throws invalid_argument if provided pixel is out of bounds
     */
    [[nodiscard]] geometry::Point getPixelCoords(int x, int y) const;
  };
}
