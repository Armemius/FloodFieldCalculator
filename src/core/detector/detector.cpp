#include "core/detector/detector.h"

#include <stdexcept>

namespace pwn::ffc::core {
  geometry::Point Detector::getPixelCoords(const int x, const int y) const {
    if (x < 0 || x >= this->m_resolution.width || y < 0 || y >= this->m_resolution.height) {
      throw std::invalid_argument("Pixel coordinates are out of bounds");
    }
    return this->calculatePixelCoords(x, y);
  }

  Detector::Detector(const geometry::Plane<int> resolution, const geometry::Plane<double> size,
                     const double rotation): m_resolution(resolution),
                                             m_size(size),
                                             m_rotation(rotation) {
  }

  geometry::Plane<int> Detector::getResolution() const {
    return this->m_resolution;
  }

  geometry::Plane<double> Detector::getSize() const {
    return this->m_size;
  }
}
