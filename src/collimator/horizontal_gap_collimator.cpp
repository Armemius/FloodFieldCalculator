#include "collimator/horizontal_gap_collimator.h"

ffc::Horizontal_gap_collimator::Horizontal_gap_collimator(double gap, double offset)
    : _gap(gap), _offset(offset) {}

bool ffc::Horizontal_gap_collimator::check_intersection(measures::coords ray_start,
                        measures::coords ray_end) const {
  
  const double X = this->_offset;
  const double T = (X - ray_start.x) / (ray_end.x - ray_start.x);
  const double Y = ray_start.y + T * (ray_end.y - ray_start.y);
  const double Z = ray_start.z + T * (ray_end.z - ray_start.z);

  return Y >= this->_gap / 2.0 || Y <= -this->_gap / 2.0;
}