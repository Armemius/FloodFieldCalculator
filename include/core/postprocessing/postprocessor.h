#pragma once

#include <opencv2/core/mat.hpp>

namespace pwn::ffc::core {
  /// Class that handles image postprocessing
  class Postprocessor {
  public:
    virtual ~Postprocessor() = default;

    /**
     * Applies postprocessing for a given OpenCV mat
     *
     * @param mat reference to OpenCV mat that will be postprocessed
     */
    virtual void process(cv::Mat &mat) = 0;
  };
}
