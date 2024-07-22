#include "detector/curved_detector.h"

#include <iostream>
#include <stdexcept>

ffc::Curved_detector::Curved_detector(double distance,
                                  measures::dimensions<int> resolution,
                                  measures::dimensions<double> size,
                                  measures::dimensions<double> offset,
                                  double curve_radius)
    : Detector(distance, resolution, size, offset), _curve_radius(curve_radius) {}

ffc::measures::coords
ffc::Curved_detector::calculate_coords_of_pixel(int x, int y) const {
  if (x >= this->_resolution.width || y >= this->_resolution.height) {
    throw std::out_of_range("Coordinates of pixel out of range");
  }


  const ffc::measures::dimensions<double> pixel_size = {
      this->_size.width / this->_resolution.width,
      this->_size.height / this->_resolution.height};

  
  const double X_OFFSET = this->_distance;

  const double R = this->_curve_radius;
  const double W = this->_size.width;
  const double Y_OFFSET = this->_size.height / 2 + this->_offset.height;

  const double ALPHA = (W - pixel_size.width * (2 * x + 1)) / (2 * R) ;
  const double X = R * cos(ALPHA) - R + X_OFFSET;
  const double Z = R * sin(ALPHA);
  const double Y = -Y_OFFSET + pixel_size.height * (y + 0.5);

  return {X, Y, Z};
}
