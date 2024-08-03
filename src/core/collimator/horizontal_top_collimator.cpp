#include "core/collimator/horizontal_left_collimator.h"

namespace pwn::ffc::core {
  HorizontalTopCollimator::HorizontalTopCollimator(const config::Collimator &collimator) : Collimator(collimator) {
  }

  HorizontalTopCollimator::HorizontalTopCollimator(const double distance, const double shift)
    : Collimator(distance, shift) {
  }

  bool HorizontalTopCollimator::doesIntersect(const geometry::Ray &ray) const {
    const double x = this->m_distance;
    const double t = (x - ray.start.x) / (ray.end.x - ray.start.x);
    const double y = ray.start.y + t * (ray.end.y - ray.start.y);

    return y >= this->m_shift / 2.0;
  }
}
