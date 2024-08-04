#pragma once

#include "image/image_handler.h"

namespace pwn::ffc::image {
  /// Class that handles exporting OpenCV Mat objects to TIFF images
  class TiffHandler : public ImageHandler {
  protected:
    /// Converts pixel data to be used in TIFF
    virtual void convertPixelData_(cv::Mat &mat) = 0;

  public:
    /**
     * Exports OpenCV Mat objects to TIFF format and stores resulting image inside filesystem
     *
     * @param mat reference to Mat to export
     * @param filename name of exported file (without extension)
     */
    void outputImage(const cv::Mat &mat, const std::string &filename) final;
  };
}
