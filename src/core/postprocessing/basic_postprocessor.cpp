#include "core/postprocessing/basic_postprocessor.h"

#include <opencv2/imgproc.hpp>

namespace pwn::ffc::core {
  BasicPostprocessor::BasicPostprocessor(const int blur_radius, const int target_width,
                                         const int target_height, bool invert): m_blur_radius(blur_radius),
                                                                                m_target_width(target_width),
                                                                                m_target_height(target_height),
                                                                                m_invert(invert) {
  }

  void BasicPostprocessor::process(cv::Mat &mat) {
    // if (mat.type() != CV_8UC1 && mat.type() != CV_16UC1) {
    //   throw std::runtime_error("Basic postprocessing isn't allowed for floating type pixel data");
    // }
    resize(mat, mat, cv::Size(m_target_width, m_target_height), 0, 0, cv::INTER_LINEAR);

    if (m_blur_radius <= 0) {
      m_blur_radius = 1;
    }
    if (m_blur_radius % 2 == 0) {
      ++m_blur_radius;
    }
    GaussianBlur(mat, mat, cv::Size(m_blur_radius, m_blur_radius), 0);
  }
}
