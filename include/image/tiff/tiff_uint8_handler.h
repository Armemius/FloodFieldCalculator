#pragma once
#include "tiff_handler.h"
#include "core/postprocessing/postprocessor.h"

namespace pwn::ffc::image {
  class TiffUint8Handler final : public TiffHandler {
    std::unique_ptr<core::Postprocessor> m_postprocessor;

  protected:
    void convertPixelData_(cv::Mat &mat) override;

  public:
    explicit TiffUint8Handler(std::unique_ptr<core::Postprocessor> postprocessor);
  };
}
