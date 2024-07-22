#include "filter/gauss_filter.h"

#include <iostream>

double ffc::Gauss_filter::_function(const double x, const double k) const {
  return -x + _offset + _thickness - _depth * exp(-(k * x / _sigma) * (k * x / _sigma));
}

double ffc::Gauss_filter::_derivative(const double x, const double k) const {
  return -1 - (2 * _depth * k * k * x) / (exp((k * x / _sigma) * (k * x / _sigma)) * _sigma * _sigma);
}

double ffc::Gauss_filter::_solve_equation(double k, double prev_value, double eps) const {
  if (prev_value == FIRST_ITERATION_VALUE) {
    prev_value = _offset + _thickness / 2;
  }
  double new_value = prev_value - _function(prev_value, k) / _derivative(prev_value, k);
  if (std::abs(new_value - prev_value) < eps) {
    return new_value;
  }
  return _solve_equation(k, new_value);
}

ffc::Gauss_filter::Gauss_filter(double thickness, double offset, double sigma, double depth, Attenuation attenuation): Filter(attenuation),
  _thickness(
    thickness), _offset(
    offset), _sigma(
    sigma), _depth(
    depth) {}

double ffc::Gauss_filter::calculate_intersection_length(measures::coords ray_start, measures::coords ray_end) const {
  const double X_1 = this->_offset;
  const double T_1 = (X_1 - ray_start.x) / (ray_end.x - ray_start.x);
  const double Y_1 = ray_start.y + T_1 * (ray_end.y - ray_start.y);
  const double Z_1 = ray_start.z + T_1 * (ray_end.z - ray_start.z);

  const double K = (ray_end.y - ray_start.y) / (ray_end.x - ray_start.x);

  const double X_2 = this->_solve_equation(K);
  const double T_2 = (X_2 - ray_start.x) / (ray_end.x - ray_start.x);
  const double Y_2 = ray_start.y + T_2 * (ray_end.y - ray_start.y);
  const double Z_2 = ray_start.z + T_2 * (ray_end.z - ray_start.z);

  return measures::distance({X_1, Y_1, Z_1}, {X_2, Y_2, Z_2});
}

bool ffc::Gauss_filter::check_intersection(measures::coords ray_start, measures::coords ray_end) const {
  return true;
}
