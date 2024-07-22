#include "filter/cylindrical_filter.h"
#include <iostream>

ffc::Cylindrical_filter::Cylindrical_filter(double thickness, double offset,
                                            double radius,
                                            Attenuation attenuation)
    : _thickness(thickness), _offset(offset), _radius(radius),
      Filter(attenuation) {}

double ffc::Cylindrical_filter::calculate_intersection_length(
    measures::coords ray_start, measures::coords ray_end) const {

  /*
    Same as slab filter, but with cylindrical cut
  */

 // TODO Checks for values

  const double R = this->_radius;

  const double X_1 = this->_offset;
  const double X_2 = this->_offset + this->_thickness;

  const double T_1 = (X_1 - ray_start.x) / (ray_end.x - ray_start.x);
  const double T_2 = (X_2 - ray_start.x) / (ray_end.x - ray_start.x);

  const double Y_1 = ray_start.y + T_1 * (ray_end.y - ray_start.y);
  const double Y_2 = ray_start.y + T_2 * (ray_end.y - ray_start.y);

  const double Z_1 = ray_start.z + T_1 * (ray_end.z - ray_start.z);
  const double Z_2 = ray_start.z + T_2 * (ray_end.z - ray_start.z);

  if (X_1 * X_1 + Z_1 * Z_1 <= R * R) {
    const double ALPHA = ray_end.x - ray_start.x;
    const double BETA = ray_end.y - ray_start.y;

    const double A = ALPHA * ALPHA + BETA * BETA;
    const double B = 2 * ray_start.x * ALPHA + 2 * ray_start.z * BETA;
    const double C = ray_start.x * ray_start.x + ray_start.z * ray_start.z - R * R;

    const double D = B * B - 4 * A * C;

    // const double T_A = (-B - sqrt(D)) / (2 * A);
    const double T_B = (-B + sqrt(D)) / (2 * A);

    // const double X_A = ray_start.x + T_A * (ray_end.x - ray_start.x);
    // const double Y_A = ray_start.y + T_A * (ray_end.y - ray_start.y);
    // const double Z_A = ray_start.z + T_A * (ray_end.z - ray_start.z);

    const double X_B = ray_start.x + T_B * (ray_end.x - ray_start.x);
    const double Y_B = ray_start.y + T_B * (ray_end.y - ray_start.y);
    const double Z_B = ray_start.z + T_B * (ray_end.z - ray_start.z);

    // const measures::coords P_1 = {X_A, Y_A, Z_A};
    const measures::coords P_2 = {X_B, Y_B, Z_B};

    // std::cout << P_1 << std::endl << P_2 << std::endl << std::endl;
    return measures::distance(P_2, {X_2, Y_2, Z_2});
  }

  return measures::distance({X_1, Y_1, Z_1}, {X_2, Y_2, Z_2});
}

bool ffc::Cylindrical_filter::check_intersection(measures::coords ray_start,
                                          measures::coords ray_end) const {
  return ray_start.x - ray_end.x != 0;
}
