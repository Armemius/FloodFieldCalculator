#pragma once

#include "core/postprocessing/postprocessor.h"

namespace pwn::ffc::core {
  class BasicPostprocessor final : public Postprocessor {
    int m_blur_radius;
    int m_target_width;
    int m_target_height;
    bool m_invert;

  public:
    BasicPostprocessor(int blur_radius, int target_width, int target_height, bool invert);

    void process(cv::Mat &mat) override;
  };
}
