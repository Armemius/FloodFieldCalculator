#pragma once

#include <opencv2/core/mat.hpp>

namespace pwn::ffc::core {
  class Postprocessor {
  public:
    virtual ~Postprocessor() = default;

    virtual void process(cv::Mat &mat) = 0;
  };
}
