#pragma once

namespace pwn::ffc::geometry {
  struct Point {
    double x;
    double y;
    double z;
  };

  double distance(const Point &a, const Point &b);
}
