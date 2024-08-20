#include "core/calculator/attenuation_calculator.h"

#include <spdlog/spdlog.h>

namespace pwn::ffc::core {
  AttenuationCalculator::AttenuationCalculator(const bool logarithmize,
                                               const cv::Mat &rescale_coefficient)
    : m_logarithmize(logarithmize),
      m_rescale_coefficient(rescale_coefficient) {
  }

  cv::Mat AttenuationCalculator::process() {
    const auto [width, height] = m_detector->getResolution();
    cv::Mat res(height, width, CV_64FC1);
    for (int row = 0; row < res.rows; ++row) {
      for (int col = 0; col < res.cols; ++col) {
        if ((res.cols * row + col) % 10000 == 0 && res.cols * row + col > 0) {
          spdlog::info("{}% done", std::to_string((res.cols * row + col) * 100.0 / res.total()));
        }

        double attenuation = 0;
        const auto &registration_coefficients = m_spectrum.getRegistrationCoefficients();
        for (size_t channel_index = 0; channel_index < registration_coefficients.size(); ++channel_index) {
          const double channel_force = registration_coefficients[channel_index];
          double channel_attenuation = 0;
          for (const auto &filter: m_filters) {
            if (!filter->doesIntersect(geometry::Ray(m_detector->getPixelCoords(col, row)))) {
              continue;
            }
            const auto filter_material = filter->getMaterial();
            const auto filter_attenuation_coefficients = m_spectrum.getMaterialCoefficients(filter_material);
            const auto intersection_length = filter->calculateIntersectionDistance(
              geometry::Ray(m_detector->getPixelCoords(col, row)));
            channel_attenuation += intersection_length * filter_attenuation_coefficients[channel_index];
          }
          attenuation += channel_force * std::exp(-channel_attenuation);
        }

        for (const auto &it: m_collimators) {
          if (it->doesIntersect(geometry::Ray(m_detector->getPixelCoords(col, row)))) {
            attenuation = std::exp(-INFINITY);
          }
        }
        if (m_logarithmize) {
          attenuation = -std::log(attenuation);
        }
        res.at<double>(row, col) = attenuation;
      }
    }
    if (m_logarithmize) {
      res = res.mul(m_rescale_coefficient);
    }
    spdlog::info("100% done");
    return res;
  }
}
