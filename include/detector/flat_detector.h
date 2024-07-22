#pragma once

#include "detector/detector.h"

namespace ffc {

class Flat_detector : public Detector {
public:
  Flat_detector(double distance, measures::dimensions<int> resolution,
                measures::dimensions<double> size,
                measures::dimensions<double> offset);

  measures::coords calculate_coords_of_pixel(int x,
                                                     int y) const override;
};

} // namespace ffc
