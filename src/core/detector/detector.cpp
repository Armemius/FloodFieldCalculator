#include "core/detector/detector.h"

#include <stdexcept>

namespace pwn::ffc::core {
  geometry::Point Detector::getPixelCoords(const int x, const int y) const {
    if (x < 0 || x >= this->resolution_.width || y < 0 || y >= this->resolution_.height) {
      throw std::invalid_argument("Pixel coordinates are out of bounds");
    }
    return this->calculatePixelCoords(x, y);
  }

  Detector::Detector(const double distance, const geometry::Plane<int> resolution,
                     const geometry::Plane<double> size): distance_(distance),
                                                          resolution_(resolution),
                                                          size_(size) {
  }
}
