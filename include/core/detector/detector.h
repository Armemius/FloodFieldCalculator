#pragma once

#include "geometry/plane.h"
#include "geometry/point.h"

namespace pwn::ffc::core {
  class Detector {
  protected:
    const double distance_;
    const geometry::Plane<int> resolution_;
    const geometry::Plane<double> size_;

    [[nodiscard]] virtual geometry::Point calculatePixelCoords(int x, int y) const = 0;

  public:
    virtual ~Detector() = default;

    Detector(double distance, geometry::Plane<int> resolution, geometry::Plane<double> size);

    [[nodiscard]] geometry::Point getPixelCoords(int x, int y) const;
  };
}
