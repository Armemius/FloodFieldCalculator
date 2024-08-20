#include "core/filter/dist_filter.h"
#include "geometry/ray.h"

namespace pwn::ffc::core {
  DistFilter::DistFilter(const config::Filter &filter) : Filter(filter.material,
                                                                filter.id,
                                                                filter.rotation),
                                                         m_distance(filter.distance),
                                                         m_radius(filter.radius) {
  }

  DistFilter::DistFilter(const std::string &material,
                         const std::optional<std::string> &id,
                         const double distance,
                         const double radius,
                         const double rotation) : Filter(material, id, rotation),
                                                m_distance(distance),
                                                m_radius(radius) {
  }

  double DistFilter::calculateIntersectionDistance(const geometry::Ray &ray) const {
    const auto ray_transformed = pwn::ffc::geometry::rotate(ray, m_rotation);

    const double k = (ray_transformed.end.z - ray_transformed.start.z) / (ray_transformed.end.x - ray_transformed.start.x);

    const double a = k * k + 1;
    const double b = -2 * this->m_distance;
    const double c = this->m_distance * this->m_distance - this->m_radius * this->m_radius;

    const double d = b * b - 4 * a * c;

    if (d < 0) {
      return 0;
    }

    const double x_1 = (-b - sqrt(d)) / (2 * a);
    const double x_2 = (-b + sqrt(d)) / (2 * a);

    const double t_1 = (x_1 - ray_transformed.start.x) / (ray_transformed.end.x - ray_transformed.start.x);
    const double t_2 = (x_2 - ray_transformed.start.x) / (ray_transformed.end.x - ray_transformed.start.x);

    const double y_1 = ray_transformed.start.y + t_1 * (ray_transformed.end.y - ray_transformed.start.y);
    const double y_2 = ray_transformed.start.y + t_2 * (ray_transformed.end.y - ray_transformed.start.y);

    const double z_1 = ray_transformed.start.z + t_1 * (ray_transformed.end.z - ray_transformed.start.z);
    const double z_2 = ray_transformed.start.z + t_2 * (ray_transformed.end.z - ray_transformed.start.z);

    const geometry::Point p_1 = {x_1, y_1, z_1};
    const geometry::Point p_2 = {x_2, y_2, z_2};

    return distance(p_1, p_2);
  }

  bool DistFilter::doesIntersect(const geometry::Ray &ray) const {
    const auto ray_transformed = rotate(ray, m_rotation);

    const double k = (ray_transformed.end.z - ray_transformed.start.z) / (ray_transformed.end.x - ray_transformed.start.x);

    const double a = k * k + 1;
    const double b = -2 * this->m_distance;
    const double c = this->m_distance * this->m_distance - this->m_radius * this->m_radius;

    const double d = b * b - 4 * a * c;

    return d >= 0;
  }
}
