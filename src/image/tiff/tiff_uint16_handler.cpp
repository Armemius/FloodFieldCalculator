#include "image/tiff/tiff_uint16_handler.h"

namespace pwn::ffc::image {
  void TiffUint16Handler::convertPixelData_(cv::Mat &mat) {
    mat.convertTo(mat, CV_16UC1);
  }
}
