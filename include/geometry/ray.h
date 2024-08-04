#pragma once

#include "point.h"

namespace pwn::ffc::geometry {
  /// Structs representing geometric ray
  struct Ray {
    explicit Ray(const Point &a);

    Ray(const Point &a, const Point &b);

    Point start;
    Point end;
  };
}
