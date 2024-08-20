#include "core/filter/bowtie_gauss_filter.h"

namespace pwn::ffc::core {
  double BowtieGaussFilter::function(const double x, const double k) const {
    return -x + this->m_distance + this->m_thickness
           - this->m_depth * exp(-(k * x / this->m_sigma) * (k * x / this->m_sigma));
  }

  double BowtieGaussFilter::derivative(const double x, const double k) const {
    return -1 - 2 * this->m_depth * k * k * x
           / (exp(k * x / this->m_sigma * (k * x / this->m_sigma)) * this->m_sigma * this->m_sigma);
  }

  // NOLINTNEXTLINE
  double BowtieGaussFilter::solveEquation(const double k, double prev_value, double eps) const {
    if (prev_value == FIRST_ITERATION_VALUE) {
      prev_value = m_distance + m_thickness / 2;
    }
    const double new_value = prev_value - function(prev_value, k) / derivative(prev_value, k);
    if (std::abs(new_value - prev_value) < eps) {
      return new_value;
    }
    return solveEquation(k, new_value);
  }

  BowtieGaussFilter::BowtieGaussFilter(const config::Filter &filter) : Filter(filter.material, filter.id, filter.rotation),
                                                                       m_thickness(filter.thickness),
                                                                       m_distance(filter.distance),
                                                                       m_sigma(filter.sigma),
                                                                       m_depth(filter.depth) {
  }

  BowtieGaussFilter::BowtieGaussFilter(const std::string &material,
                                       const std::optional<std::string> &id,
                                       const double distance,
                                       const double thickness,
                                       const double sigma,
                                       const double depth,
                                       const double rotation): Filter(material, id, rotation),
                                                            m_thickness(thickness),
                                                            m_distance(distance),
                                                            m_sigma(sigma),
                                                            m_depth(depth) {
  }


  double BowtieGaussFilter::calculateIntersectionDistance(const geometry::Ray &ray) const {
    const auto ray_transformed = rotate(ray, m_rotation);

    const double x_1 = this->m_distance;
    const double t_1 = (x_1 - ray_transformed.start.x) / (ray_transformed.end.x - ray_transformed.start.x);
    const double y_1 = ray_transformed.start.y + t_1 * (ray_transformed.end.y - ray_transformed.start.y);
    const double z_1 = ray_transformed.start.z + t_1 * (ray_transformed.end.z - ray_transformed.start.z);

    const double k = (ray_transformed.end.y - ray_transformed.start.y) / (ray_transformed.end.x - ray_transformed.start.x);

    const double x_2 = this->solveEquation(k);
    const double t_2 = (x_2 - ray_transformed.start.x) / (ray_transformed.end.x - ray_transformed.start.x);
    const double y_2 = ray_transformed.start.y + t_2 * (ray_transformed.end.y - ray_transformed.start.y);
    const double z_2 = ray_transformed.start.z + t_2 * (ray_transformed.end.z - ray_transformed.start.z);

    return geometry::distance({x_1, y_1, z_1}, {x_2, y_2, z_2});
  }

  bool BowtieGaussFilter::doesIntersect(const geometry::Ray &ray) const {
    return ray.start.x != ray.end.x;
  }
}
