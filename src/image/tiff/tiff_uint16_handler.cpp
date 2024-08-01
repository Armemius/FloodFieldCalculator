#include "image/tiff/tiff_uint16_handler.h"

namespace pwn::ffc::image {
  void TiffUint16Handler::convertPixelData_(cv::Mat &mat) {
    threshold(mat, mat, UINT16_MAX, UINT16_MAX, cv::THRESH_TRUNC);
    mat.convertTo(mat, CV_16UC1);
  }
}
