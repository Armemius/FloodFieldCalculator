#include "core/collimator/horizontal_right_collimator.h"

namespace pwn::ffc::core {
  HorizontalBottomCollimator::HorizontalBottomCollimator(const config::Collimator &collimator) : Collimator(collimator) {
  }

  HorizontalBottomCollimator::HorizontalBottomCollimator(const double distance, const double shift)
    : Collimator(distance, shift) {
  }

  bool HorizontalBottomCollimator::doesIntersect(const geometry::Ray &ray) const {
    const double x = this->m_distance;
    const double t = (x - ray.start.x) / (ray.end.x - ray.start.x);
    const double y = ray.start.y + t * (ray.end.y - ray.start.y);

    return y <= -this->m_shift / 2.0;
  }
}
