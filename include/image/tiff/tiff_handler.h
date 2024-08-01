#pragma once

#include "image/image_handler.h"

namespace pwn::ffc::image {
  class TiffHandler : public ImageHandler {
  protected:
    virtual void convertPixelData_(cv::Mat &mat) = 0;
  public:
    void outputImage(const cv::Mat &mat, const std::string &filename) final;
  };
}
