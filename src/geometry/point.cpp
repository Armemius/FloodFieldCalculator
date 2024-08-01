#include "geometry/point.h"

#include <cmath>

namespace pwn::ffc::geometry {
  double distance(const Point &a, const Point &b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
  }
}
