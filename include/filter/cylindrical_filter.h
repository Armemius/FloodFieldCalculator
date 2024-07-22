#pragma once

#include "filter/filter.h"

namespace ffc {

class Cylindrical_filter : public Filter {
  const double _thickness;
  const double _offset;
  const double _radius;

public:
  Cylindrical_filter(double thickness, double offset, double radius, Attenuation attenuation);

  double
  calculate_intersection_length(measures::coords ray_start,
                                measures::coords ray_end) const override;
  bool check_intersection(measures::coords ray_start,
                                  measures::coords ray_end) const override;
};

} // namespace ffc
