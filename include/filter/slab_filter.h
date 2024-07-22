#pragma once

#include "filter/filter.h"

namespace ffc {

class Slab_filter : public Filter {
  const double _thickness;
  const double _offset;

public:
  Slab_filter(double thickness, double offset, Attenuation attenuation);

  double
  calculate_intersection_length(measures::coords ray_start,
                                measures::coords ray_end) const override;
  bool check_intersection(measures::coords ray_start,
                                  measures::coords ray_end) const override;
};

} // namespace ffc
