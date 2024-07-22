#include "filter/curved_filter.h"
#include <iostream>

ffc::Curved_filter::Curved_filter(double min_thickness, double max_thickness,
                                  double offset, double radius,
                                  Attenuation attenuation)
    : _min_thickness(min_thickness), _max_thickness(max_thickness),
      _offset(offset), _radius(radius), Filter(attenuation) {}

double ffc::Curved_filter::calculate_intersection_length(
    measures::coords ray_start, measures::coords ray_end) const {
  const double ALPHA = this->_radius;
  const double X_F = this->_offset;
  const double D_MIN = this->_min_thickness;
  const double D_MAX = this->_max_thickness;

  // Coefficients for the line
  const double A = ray_end.x - ray_start.x;
  const double C = ray_end.z - ray_start.z;

  const double A_0 = ALPHA * C * C;
  if (A_0 == 0) {
    return D_MIN;
  }

  const double B_0 = 2 * ALPHA * C * ray_start.z - A;
  const double C_0 =
      X_F + D_MIN + ALPHA * ray_start.z * ray_start.z - ray_start.x;

  const double D = B_0 * B_0 - 4 * A_0 * C_0;

  /*
    Maximum 4 points of intersection
    The line is parametric equation
    T_1, T_4 are parameters for intersection points with flat surface
    T_2, T_3 are parameters for intersection points with parabolic surface
    Parameters are ordered the way intersection points are located by the ray
    path E.g. ray passes through intersection point formed by parameter T_1,
    then T_2, then T_3, then T_4
  */

  const double X_1 = this->_offset;
  const double X_4 = this->_offset + this->_max_thickness;

  const double T_1 = (X_1 - ray_start.x) / (ray_end.x - ray_start.x);
  const double T_4 = (X_4 - ray_start.x) / (ray_end.x - ray_start.x);

  const double Y_1 = ray_start.y + T_1 * (ray_end.y - ray_start.y);
  const double Y_4 = ray_start.y + T_4 * (ray_end.y - ray_start.y);

  const double Z_1 = ray_start.z + T_1 * (ray_end.z - ray_start.z);
  const double Z_4 = ray_start.z + T_4 * (ray_end.z - ray_start.z);

  const measures::coords P_1 = {X_1, Y_1, Z_1};
  const measures::coords P_4 = {X_4, Y_4, Z_4};

  if (D > 0) {
    const double T_2 = (-B_0 - sqrt(D)) / (2 * A_0);
    const double T_3 = (-B_0 + sqrt(D)) / (2 * A_0);

    const double X_2 = ray_start.x + T_2 * (ray_end.x - ray_start.x);
    const double X_3 = ray_start.x + T_3 * (ray_end.x - ray_start.x);

    const double Y_2 = ray_start.y + T_2 * (ray_end.y - ray_start.y);
    const double Y_3 = ray_start.y + T_3 * (ray_end.y - ray_start.y);

    const double Z_2 = ray_start.z + T_2 * (ray_end.z - ray_start.z);
    const double Z_3 = ray_start.z + T_3 * (ray_end.z - ray_start.z);

    const measures::coords P_2 = {X_2, Y_2, Z_2};
    const measures::coords P_3 = {X_3, Y_3, Z_3};

    if (X_F <= X_4 &&
        X_4 <= std::min(X_F + D_MAX, X_F + D_MIN + ALPHA * Z_4 * Z_4)) {
      return measures::distance(P_1, P_2) + measures::distance(P_3, P_4);
    }
    return measures::distance(P_1, P_2);
  }
  return measures::distance(P_1, P_4);
}

bool ffc::Curved_filter::check_intersection(measures::coords ray_start,
                                            measures::coords ray_end) const {
  return ray_start.x - ray_end.x != 0;
}