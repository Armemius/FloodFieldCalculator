#include "core/collimator/vertical_right_collimator.h"

namespace pwn::ffc::core {
  VerticalRightCollimator::VerticalRightCollimator(const config::Collimator &collimator) : Collimator(collimator) {
  }

  VerticalRightCollimator::VerticalRightCollimator(const double distance, const double shift)
    : Collimator(distance, shift) {
  }

  bool VerticalRightCollimator::doesIntersect(const geometry::Ray &ray) const {
    const double x = this->m_distance;
    const double t = (x - ray.start.x) / (ray.end.x - ray.start.x);
    const double z = ray.start.z + t * (ray.end.z - ray.start.z);

    return z <= -this->m_shift / 2.0;
  }
}
