#include "core/calculator/distance_calculator.h"

namespace pwn::ffc::core {
  cv::Mat DistanceCalculator::process() {
    const auto [width, height] = m_detector->getResolution();
    cv::Mat res(height, width, CV_64FC1);

    for (int row =  0; row < res.rows; ++row) {
      for (int col =  0; col < res.cols; ++col) {
        double distance = 0;
        for (const auto &it : m_filters) {
          distance += it->calculateIntersectionDistance(geometry::Ray(m_detector->getPixelCoords(col, row)));
        }
        res.at<double>(row, col) = distance;
      }
    }
    return res;
  }
}
