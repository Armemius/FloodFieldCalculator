#include "image/tiff/tiff_handler.h"

namespace pwn::ffc::image {
  void TiffHandler::outputImage(const cv::Mat &mat, const std::string &filename) {
    std::string tiffFilename = filename;
    if (filename.substr(filename.find_last_of('.') + 1) != "tiff" &&
        filename.substr(filename.find_last_of('.') + 1) != "tif") {
      tiffFilename += ".tiff";
    }
    cv::Mat convertedImage;
    mat.copyTo(convertedImage);
    convertPixelData_(convertedImage);
    if (!imwrite(tiffFilename, convertedImage)) {
      throw std::runtime_error("Could not save the image as TIFF.");
    }
  }
}
