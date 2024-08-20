#include "core/detector/flat_detector.h"

namespace pwn::ffc::core {
  FlatDetector::FlatDetector(const geometry::Plane<int> &resolution,
                             const geometry::Plane<double> &size,
                             double distance,
                             double rotation) : Detector(resolution, size, rotation), m_distance(distance) {
  }

  geometry::Point FlatDetector::calculatePixelCoords(int x, int y) const {
    const geometry::Plane<double> pixel_size = {
      this->m_size.width / this->m_resolution.width,
      this->m_size.height / this->m_resolution.height
    };

    const double z_offset = this->m_size.width / 2;
    const double y_offset = this->m_size.height / 2;

    const double z_position = -z_offset + pixel_size.width * (x + 0.5);
    const double y_position = -y_offset + pixel_size.height * (y + 0.5);

    const auto point = geometry::Point{.x=this->m_distance, .y=y_position, .z=z_position};
    return rotate(point, -m_rotation);
  }
}
