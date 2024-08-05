#pragma once
#include "tiff_handler.h"

namespace pwn::ffc::image {
  class TiffFloat32Handler final : public TiffHandler {
  protected:
    void convertPixelData_(cv::Mat &mat) override;
  public:
    TiffFloat32Handler() = default;
  };
}
