#pragma once

#include "detector.h"

namespace pwn::ffc::core {
  class FlatDetector final : public Detector {
    double m_distance;

  public:
    FlatDetector(const geometry::Plane<int> &resolution, const geometry::Plane<double> &size, double distance);

  protected:
    [[nodiscard]] geometry::Point calculatePixelCoords(int x, int y) const override;
  };
}
