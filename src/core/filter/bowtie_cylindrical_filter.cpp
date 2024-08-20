#include "core/filter/bowtie_cylindrical_filter.h"

namespace pwn::ffc::core {
  BowtieCylindricalFilter::BowtieCylindricalFilter(const config::Filter &filter) : Filter(filter.material,
                                                                                          filter.id,
                                                                                          filter.rotation),
                                                                                   m_thickness(filter.thickness),
                                                                                   m_distance(filter.distance),
                                                                                   m_radius(filter.radius) {
  }

  BowtieCylindricalFilter::BowtieCylindricalFilter(const std::string &material,
                                                   const std::optional<std::string> &id,
                                                   const double distance,
                                                   const double thickness,
                                                   const double radius,
                                                   const double rotation) : Filter(material, id, rotation),
                                                                          m_thickness(thickness),
                                                                          m_distance(distance),
                                                                          m_radius(radius) {
  }

  double BowtieCylindricalFilter::calculateIntersectionDistance(const geometry::Ray &ray) const {
    const auto ray_transformed = rotate(ray, m_rotation);

    // Same as slab filter, but with cylindrical cut

    const double r = this->m_radius;

    const double x_1 = this->m_distance;
    const double x_2 = this->m_distance + this->m_thickness;

    const double t_1 = (x_1 - ray_transformed.start.x) / (ray_transformed.end.x - ray_transformed.start.x);
    const double t_2 = (x_2 - ray_transformed.start.x) / (ray_transformed.end.x - ray_transformed.start.x);

    const double y_1 = ray_transformed.start.y + t_1 * (ray_transformed.end.y - ray_transformed.start.y);
    const double y_2 = ray_transformed.start.y + t_2 * (ray_transformed.end.y - ray_transformed.start.y);

    const double z_1 = ray_transformed.start.z + t_1 * (ray_transformed.end.z - ray_transformed.start.z);
    const double z_2 = ray_transformed.start.z + t_2 * (ray_transformed.end.z - ray_transformed.start.z);

    if (x_1 * x_1 + z_1 * z_1 <= r * r) {
      const double alpha = ray_transformed.end.x - ray_transformed.start.x;
      const double beta = ray_transformed.end.y - ray_transformed.start.y;

      const double a = alpha * alpha + beta * beta;
      const double b = 2 * ray_transformed.start.x * alpha + 2 * ray_transformed.start.z * beta;
      const double c = ray_transformed.start.x * ray_transformed.start.x + ray_transformed.start.z * ray_transformed.start.z - r * r;

      const double d = b * b - 4 * a * c;

      const double t_b = (-b + sqrt(d)) / (2 * a);

      const double x_b = ray_transformed.start.x + t_b * (ray_transformed.end.x - ray_transformed.start.x);
      const double y_b = ray_transformed.start.y + t_b * (ray_transformed.end.y - ray_transformed.start.y);
      const double z_b = ray_transformed.start.z + t_b * (ray_transformed.end.z - ray_transformed.start.z);

      const geometry::Point p = {x_b, y_b, z_b};

      return distance(p, {x_2, y_2, z_2});
    }

    return geometry::distance({x_1, y_1, z_1}, {x_2, y_2, z_2});
  }

  bool BowtieCylindricalFilter::doesIntersect(const geometry::Ray &ray) const {
    return ray.start.x != ray.end.x;
  }
}
