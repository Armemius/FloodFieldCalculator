#include "core/collimator/horizontal_symmetrical_collimator.h"

namespace pwn::ffc::core {
  HorizontalSymmetricalCollimator::HorizontalSymmetricalCollimator(
    const config::Collimator &collimator): Collimator(collimator) {
  }

  HorizontalSymmetricalCollimator::HorizontalSymmetricalCollimator(const double distance, const double shift)
    : Collimator(distance, shift) {
  }

  bool HorizontalSymmetricalCollimator::doesIntersect(const geometry::Ray &ray) const {
    const double x = this->m_distance;
    const double t = (x - ray.start.x) / (ray.end.x - ray.start.x);
    const double y = ray.start.y + t * (ray.end.y - ray.start.y);

    return y >= this->m_shift / 2.0 || y <= -this->m_shift / 2.0;
  }
}
