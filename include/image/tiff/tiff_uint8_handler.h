#pragma once
#include "tiff_handler.h"

namespace pwn::ffc::image {
  class TiffUint8Handler final : public TiffHandler {
  protected:
    void convertPixelData_(cv::Mat &mat) override;
  };
}
