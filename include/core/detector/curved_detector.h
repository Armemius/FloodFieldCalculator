#pragma once

#include "detector.h"

namespace pwn::ffc::core {
  class CurvedDetector final : public Detector {
    double m_radius;
    double m_distance;
  public:
    CurvedDetector(const geometry::Plane<int> &resolution, const geometry::Plane<double> &size, double distance, double radius);

  protected:
    [[nodiscard]] geometry::Point calculatePixelCoords(int x, int y) const override;
  };
}
