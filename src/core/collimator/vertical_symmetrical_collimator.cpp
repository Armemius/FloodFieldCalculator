#include "core/collimator/vertical_symmetrical_collimator.h"

namespace pwn::ffc::core {
  VerticalSymmetricalCollimator::VerticalSymmetricalCollimator(
    const config::Collimator &collimator): Collimator(collimator) {
  }

  VerticalSymmetricalCollimator::VerticalSymmetricalCollimator(const double distance, const double shift)
    : Collimator(distance, shift) {
  }

  bool VerticalSymmetricalCollimator::doesIntersect(const geometry::Ray &ray) const {
    const double x = this->m_distance;
    const double t = (x - ray.start.x) / (ray.end.x - ray.start.x);
    const double z = ray.start.z + t * (ray.end.z - ray.start.z);

    return z >= this->m_shift / 2.0 || z <= -this->m_shift / 2.0;
  }
}
