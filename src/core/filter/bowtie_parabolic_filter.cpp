#include "core/filter/bowtie_parabolic_filter.h"

namespace pwn::ffc::core {
  BowtieParabolicFilter::BowtieParabolicFilter(const config::Filter &filter) : Filter(filter.material, filter.id),
                                                                               m_distance(filter.distance),
                                                                               m_min_thickness(filter.min_thickness),
                                                                               m_max_thickness(filter.max_thickness),
                                                                               m_radius(filter.radius) {
  }

  BowtieParabolicFilter::BowtieParabolicFilter(const std::string &material,
                                               const std::optional<std::string> &id,
                                               const double distance,
                                               const double min_thickness,
                                               const double max_thickness,
                                               const double radius): Filter(material, id),
                                                                     m_distance(distance),
                                                                     m_min_thickness(min_thickness),
                                                                     m_max_thickness(max_thickness),
                                                                     m_radius(radius) {
  }

  double BowtieParabolicFilter::calculateIntersectionDistance(const geometry::Ray &ray) const {
    const double alpha = this->m_radius;
    const double x_f = this->m_distance;
    const double d_min = this->m_min_thickness;
    const double d_max = this->m_max_thickness;

    // Coefficients for the line
    const double a = ray.end.x - ray.start.x;
    const double c = ray.end.z - ray.start.z;

    const double a_0 = alpha * c * c;
    if (a_0 == 0) {
      return d_min;
    }

    const double b_0 = 2 * alpha * c * ray.start.z - a;
    const double c_0 =
        x_f + d_min + alpha * ray.start.z * ray.start.z - ray.start.x;

    const double d = b_0 * b_0 - 4 * a_0 * c_0;

    /*
      Maximum 4 points of intersection
      The line is parametric equation
      T_1, T_4 are parameters for intersection points with flat surface
      T_2, T_3 are parameters for intersection points with parabolic surface
      Parameters are ordered the way intersection points are located by the ray
      path E.g. ray passes through intersection point formed by parameter T_1,
      then T_2, then T_3, then T_4
    */

    const double x_1 = this->m_distance;
    const double x_4 = this->m_distance + this->m_max_thickness;

    const double t_1 = (x_1 - ray.start.x) / (ray.end.x - ray.start.x);
    const double t_4 = (x_4 - ray.start.x) / (ray.end.x - ray.start.x);

    const double y_1 = ray.start.y + t_1 * (ray.end.y - ray.start.y);
    const double y_4 = ray.start.y + t_4 * (ray.end.y - ray.start.y);

    const double z_1 = ray.start.z + t_1 * (ray.end.z - ray.start.z);
    const double z_4 = ray.start.z + t_4 * (ray.end.z - ray.start.z);

    const geometry::Point p_1 = {x_1, y_1, z_1};
    const geometry::Point p_4 = {x_4, y_4, z_4};

    if (d > 0) {
      const double t_2 = (-b_0 - sqrt(d)) / (2 * a_0);
      const double t_3 = (-b_0 + sqrt(d)) / (2 * a_0);

      const double x_2 = ray.start.x + t_2 * (ray.end.x - ray.start.x);
      const double x_3 = ray.start.x + t_3 * (ray.end.x - ray.start.x);

      const double y_2 = ray.start.y + t_2 * (ray.end.y - ray.start.y);
      const double y_3 = ray.start.y + t_3 * (ray.end.y - ray.start.y);

      const double z_2 = ray.start.z + t_2 * (ray.end.z - ray.start.z);
      const double z_3 = ray.start.z + t_3 * (ray.end.z - ray.start.z);

      const geometry::Point p_2 = {x_2, y_2, z_2};
      const geometry::Point p_3 = {x_3, y_3, z_3};

      if (x_f <= x_4 &&
          x_4 <= std::min(x_f + d_max, x_f + d_min + alpha * z_4 * z_4)) {
        return distance(p_1, p_2) + distance(p_3, p_4);
      }
      return distance(p_1, p_2);
    }
    return distance(p_1, p_4);
  }


  bool BowtieParabolicFilter::doesIntersect(const geometry::Ray &ray) const {
    return ray.start.x != ray.end.x;
  }
}
