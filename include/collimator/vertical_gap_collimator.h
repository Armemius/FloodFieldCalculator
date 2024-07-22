#pragma once

#include "collimator/collimator.h"
#include "measures.h"

namespace ffc {
  class Vertical_gap_collimator : public Collimator {
    double _gap;
    double _offset;
    public:
      Vertical_gap_collimator(double gap, double offset);
      bool check_intersection(measures::coords ray_start, measures::coords ray_end) const override;
  };

} // namespace ffc
