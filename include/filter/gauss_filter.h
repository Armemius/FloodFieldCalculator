#pragma once

#include "filter/filter.h"

#define FIRST_ITERATION_VALUE (-1)

namespace ffc {

class Gauss_filter : public Filter {
  const double _thickness;
  const double _offset;
  const double _sigma;
  const double _depth;

  double _function(double x, double k) const;
  double _derivative(double x, double k) const;
  double _solve_equation(double k, double prev_value = FIRST_ITERATION_VALUE, double eps = 0.0001) const;

public:
  Gauss_filter(double thickness, double offset, double sigma, double depth, Attenuation attenuation);

  double
  calculate_intersection_length(measures::coords ray_start,
                                measures::coords ray_end) const override;
  bool check_intersection(measures::coords ray_start,
                                  measures::coords ray_end) const override;
};

} // namespace ffc
