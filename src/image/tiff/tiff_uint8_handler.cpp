#include "image/tiff/tiff_uint8_handler.h"

namespace pwn::ffc::image {
  void TiffUint8Handler::convertPixelData_(cv::Mat &mat) {
    threshold(mat, mat, UINT8_MAX, UINT8_MAX, cv::THRESH_TRUNC);
    mat.convertTo(mat, CV_8UC1);
    m_postprocessor->process(mat);
  }

  TiffUint8Handler::TiffUint8Handler(std::unique_ptr<core::Postprocessor> postprocessor)
    : m_postprocessor(std::move(postprocessor)) {
  }
}
