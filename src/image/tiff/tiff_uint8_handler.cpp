#include "image/tiff/tiff_uint8_handler.h"

namespace pwn::ffc::image {
  void TiffUint8Handler::convertPixelData_(cv::Mat &mat) {
    mat.convertTo(mat, CV_8UC1);
  }
}
