#include "core/collimator/collimator.h"

namespace pwn::ffc::core {
  Collimator::Collimator(const config::Collimator &collimator) : m_distance(collimator.distance),
                                                                 m_shift(collimator.shift) {
  }

  Collimator::Collimator(const double distance, const double shift) : m_distance(distance),
                                                                      m_shift(shift) {
  }
}
