#pragma once

#include <opencv2/opencv.hpp>

namespace pwn::ffc::image {
  class ImageHandler {
  public:
    virtual ~ImageHandler() = default;

    virtual void outputImage(const cv::Mat &mat, const std::string &filename) = 0;
  };
}
