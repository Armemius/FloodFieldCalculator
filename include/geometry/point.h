#pragma once

#include <cmath>

namespace pwn::ffc::geometry {
  /// Structs representing geometric point
  struct Point {
    double x;
    double y;
    double z;
  };

  double distance(const Point &a, const Point &b);

  inline Point rotate(const Point& src, const double deg) {
    return {
      .x = src.x,
      .y = (src.y * std::cos(deg * M_PI / 180.0)) - (src.z * std::sin(deg * M_PI / 180)),
      .z = (src.y * std::sin(deg * M_PI / 180.0)) + (src.z * std::cos(deg * M_PI / 180))
    };
  }
}
