#include "geometry/ray.h"

#include <cmath>

pwn::ffc::geometry::Ray::Ray(const Point &a): start({0.0, 0.0, 0.0}), end(a) {
}

pwn::ffc::geometry::Ray::Ray(const Point &a, const Point &b): start(a), end(b) {
}
