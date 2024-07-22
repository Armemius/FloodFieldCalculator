#pragma once

#include "filter/filter.h"

namespace ffc {
  class Collimator : public Filter {
    public:
      Collimator();
      double calculate_intersection_length(measures::coords ray_start, measures::coords ray_end) const override;
  };

}
