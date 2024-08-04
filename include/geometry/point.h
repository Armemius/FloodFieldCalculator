#pragma once

namespace pwn::ffc::geometry {
  /// Structs representing geometric point
  struct Point {
    double x;
    double y;
    double z;
  };

  double distance(const Point &a, const Point &b);
}
