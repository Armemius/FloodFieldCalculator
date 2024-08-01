#include "core/filter/slab_filter.h"

namespace pwn::ffc::core {
  SlabFilter::SlabFilter(const config::Filter &filter) : Filter(filter.material),
                                                         m_thickness(filter.thickness),
                                                         m_distance(filter.distance) {
  }

  SlabFilter::SlabFilter(const std::string &material, const double distance, const double thickness)
    : Filter(material), m_thickness(thickness), m_distance(distance) {
  }

  double SlabFilter::calculateIntersectionDistance(const geometry::Ray &ray) const {
    const double x_1 = this->m_distance;
    const double x_2 = this->m_distance + this->m_thickness;

    const double t_1 = (x_1 - ray.start.x) / (ray.end.x - ray.start.x);
    const double t_2 = (x_2 - ray.start.x) / (ray.end.x - ray.start.x);

    const double y_1 = ray.start.y + t_1 * (ray.end.y - ray.start.y);
    const double y_2 = ray.start.y + t_2 * (ray.end.y - ray.start.y);

    const double z_1 = ray.start.z + t_1 * (ray.end.z - ray.start.z);
    const double z_2 = ray.start.z + t_2 * (ray.end.z - ray.start.z);

    return geometry::distance({x_1, y_1, z_1}, {x_2, y_2, z_2});
  }

  bool SlabFilter::doesIntersect(const geometry::Ray &ray) const {
    return ray.start.x != ray.end.x;
  }
}
