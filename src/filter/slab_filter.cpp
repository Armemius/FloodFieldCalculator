#include "filter/slab_filter.h"

ffc::Slab_filter::Slab_filter(double thickness, double offset,
                              Attenuation attenuation)
    : _thickness(thickness), _offset(offset), Filter(attenuation) {}

double ffc::Slab_filter::calculate_intersection_length(
    measures::coords ray_start, measures::coords ray_end) const {

  /*
  Plane equation is Ax+By+Cz+D=0, A is 1, B and C are 0
  So the equation looks like x+D=0
  D is -offset
  The slab filter plane equation is x-offset=0

  Line equation is:
  x = x_1 + t * (x_2 - x_1)
  y = y_1 + t * (y_2 - y_1)
  z = z_1 + t * (z_2 - z_1)

  We need to find value of t
  x = offset
  offset = x_1 + t * (x_2 - x_1)
  t = (offset - x_1) / (x_2 - x_1)
  After we know value of t it is possible to calculate y and z
  */

  const double X_1 = this->_offset;
  const double X_2 = this->_offset + this->_thickness;

  const double T_1 = (X_1 - ray_start.x) / (ray_end.x - ray_start.x);
  const double T_2 = (X_2 - ray_start.x) / (ray_end.x - ray_start.x);

  const double Y_1 = ray_start.y + T_1 * (ray_end.y - ray_start.y);
  const double Y_2 = ray_start.y + T_2 * (ray_end.y - ray_start.y);

  const double Z_1 = ray_start.z + T_1 * (ray_end.z - ray_start.z);
  const double Z_2 = ray_start.z + T_2 * (ray_end.z - ray_start.z);

  return measures::distance({X_1, Y_1, Z_1}, {X_2, Y_2, Z_2});
}

bool ffc::Slab_filter::check_intersection(measures::coords ray_start,
                                          measures::coords ray_end) const {
  return ray_start.x - ray_end.x != 0;
}
