#pragma once

#include <opencv2/opencv.hpp>

namespace pwn::ffc::image {
  /// Class that handles exporting OpenCV Mat objects to images
  class ImageHandler {
  public:
    virtual ~ImageHandler() = default;

    /**
     * Exports OpenCV Mat objects and stores resulting image inside filesystem
     *
     * @param mat reference to Mat to export
     * @param filename name of exported file (without extension)
     */
    virtual void outputImage(const cv::Mat &mat, const std::string &filename) = 0;
  };
}
