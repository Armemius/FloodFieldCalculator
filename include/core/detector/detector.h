#pragma once

#include "geometry/plane.h"
#include "geometry/point.h"

namespace pwn::ffc::core {
  class Detector {
  protected:
    const geometry::Plane<int> m_resolution;
    const geometry::Plane<double> m_size;

    [[nodiscard]] virtual geometry::Point calculatePixelCoords(int x, int y) const = 0;

  public:
    virtual ~Detector() = default;

    Detector(geometry::Plane<int> resolution, geometry::Plane<double> size);

    [[nodiscard]] geometry::Plane<int> getResolution() const;

    [[nodiscard]] geometry::Plane<double> getSize() const;

    [[nodiscard]] geometry::Point getPixelCoords(int x, int y) const;
  };
}
