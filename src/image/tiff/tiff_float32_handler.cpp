#include "image/tiff/tiff_float32_handler.h"

namespace pwn::ffc::image {
  void TiffFloat32Handler::convertPixelData_(cv::Mat &mat) {
    mat.convertTo(mat, CV_32FC1);
  }
}
