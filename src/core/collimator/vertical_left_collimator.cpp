#include "core/collimator/vertical_left_collimator.h"

namespace pwn::ffc::core {
  VerticalLeftCollimator::VerticalLeftCollimator(const config::Collimator &collimator) : Collimator(collimator) {
  }

  VerticalLeftCollimator::VerticalLeftCollimator(const double distance, const double shift)
    : Collimator(distance, shift) {
  }

  bool VerticalLeftCollimator::doesIntersect(const geometry::Ray &ray) const {
    const double x = this->m_distance;
    const double t = (x - ray.start.x) / (ray.end.x - ray.start.x);
    const double z = ray.start.z + t * (ray.end.z - ray.start.z);

    return z >= this->m_shift / 2.0;
  }
}
