#pragma once

#include "point.h"
#include "cmath"

namespace pwn::ffc::geometry {
  /// Structs representing geometric ray
  struct Ray {
    explicit Ray(const Point &a);

    Ray(const Point &a, const Point &b);

    Point start;
    Point end;
  };

  inline Ray rotate(const Ray &src, double deg) {
    return Ray({
      .x = src.start.x,
      .y = (src.start.y * std::cos(deg * M_PI / 180.0)) - (src.start.z * std::sin(deg * M_PI / 180)),
      .z = (src.start.y * std::sin(deg * M_PI / 180.0)) + (src.start.z * std::cos(deg * M_PI / 180))
    }, {
      .x = src.end.x,
      .y = (src.end.y * std::cos(deg * M_PI / 180.0)) - (src.end.z * std::sin(deg * M_PI / 180)),
      .z = (src.end.y * std::sin(deg * M_PI / 180.0)) + (src.end.z * std::cos(deg * M_PI / 180))
    });
  }
}
