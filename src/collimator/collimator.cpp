#include "collimator/collimator.h"
#include <float.h>

ffc::Collimator::Collimator() : Filter(DBL_MAX) {}

double ffc::Collimator::calculate_intersection_length(measures::coords ray_start, measures::coords ray_end) const {
  return 0.0;
}