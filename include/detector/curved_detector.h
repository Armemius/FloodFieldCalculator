#pragma once

#include "detector/detector.h"

namespace ffc {

class Curved_detector : public Detector {
  double _curve_radius;
public:
  Curved_detector(double distance, measures::dimensions<int> resolution,
                measures::dimensions<double> size,
                measures::dimensions<double> offset,
                double curve_radius);

  measures::coords calculate_coords_of_pixel(int x,
                                                     int y) const override;
};

} // namespace ffc
