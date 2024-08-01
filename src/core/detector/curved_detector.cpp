#include "core/detector/curved_detector.h"

#include <cmath>

namespace pwn::ffc::core {
  CurvedDetector::CurvedDetector(const geometry::Plane<int> &resolution,
                                 const geometry::Plane<double> &size,
                                 const double distance,
                                 const double radius) : Detector(resolution, size), m_radius(radius),
                                                        m_distance(distance) {
  }

  geometry::Point CurvedDetector::calculatePixelCoords(const int x, const int y) const {
    const geometry::Plane<double> pixel_size = {
      this->m_size.width / this->m_resolution.width,
      this->m_size.height / this->m_resolution.height
    };

    const double x_offset = this->m_distance;

    const double r = this->m_radius;
    const double w = this->m_size.width;
    const double y_offset = this->m_size.height / 2;

    const double alpha = (w - pixel_size.width * (2 * x + 1)) / (2 * r);
    const double x_pos = r * std::cos(alpha) - r + x_offset;
    const double y_pos = -y_offset + pixel_size.height * (y + 0.5);
    const double z_pos = r * std::sin(alpha);

    return {x_pos, y_pos, z_pos};
  }
}
