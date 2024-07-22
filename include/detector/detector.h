#pragma once

#include "measures.h"

namespace ffc {

class Detector {
protected:
  // Distance from emitter to the center of detector in millimeters
  // Aka position of detector on X axis
  const double _distance;
  // Detector resolution (in pixels)
  const measures::dimensions<int> _resolution;
  // Detector size (in millimeters)
  const measures::dimensions<double> _size;
  // Detector shift relative to emitter in millimeters
  // Aka position of center of detector on Y and Z axis
  const measures::dimensions<double> _offset;

  Detector(double distance, measures::dimensions<int> resolution,
           measures::dimensions<double> size, measures::dimensions<double> offset);

public:
  virtual measures::coords calculate_coords_of_pixel(int x, int y) const = 0;
  measures::dimensions<int> get_resolution() const;
};

} // namespace ffc
