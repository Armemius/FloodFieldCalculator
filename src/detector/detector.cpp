#include "detector/detector.h"

ffc::Detector::Detector(double distance, measures::dimensions<int> resolution,
                        measures::dimensions<double> size,
                        measures::dimensions<double> offset)
    : _distance(distance), _resolution(resolution), _size(size),
      _offset(offset) {}

ffc::measures::dimensions<int> ffc::Detector::get_resolution() const {
  return this->_resolution;
}
