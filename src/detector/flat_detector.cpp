#include "detector/flat_detector.h"

#include <iostream>
#include <stdexcept>

ffc::Flat_detector::Flat_detector(double distance,
                                  measures::dimensions<int> resolution,
                                  measures::dimensions<double> size,
                                  measures::dimensions<double> offset)
    : Detector(distance, resolution, size, offset) {}

ffc::measures::coords
ffc::Flat_detector::calculate_coords_of_pixel(int x, int y) const {
  if (x >= this->_resolution.width || y >= this->_resolution.height) {
    throw std::out_of_range("Coordinates of pixel out of range");
  }

  const ffc::measures::dimensions<double> pixel_size = {
      this->_size.width / this->_resolution.width,
      this->_size.height / this->_resolution.height};

  const double Z_OFFSET = this->_size.width / 2 + this->_offset.width;
  const double Y_OFFSET = this->_size.height / 2 + this->_offset.height;

  const double Z_POSITION = -Z_OFFSET + pixel_size.width * (x + 0.5);
  const double Y_POSITION = -Y_OFFSET + pixel_size.height * (y + 0.5);

  return {this->_distance, Y_POSITION, Z_POSITION};
}
