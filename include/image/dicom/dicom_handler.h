#pragma once

#include "image/image_handler.h"

#include <dcmtk/dcmdata/dcfilefo.h>

namespace pwn::ffc::image {
  class DicomHandler : public ImageHandler {
  protected:
    virtual void setPixelData_(cv::Mat &mat, DcmDataset *dataset) = 0;

  public:
    void outputImage(const cv::Mat &mat, const std::string &filename) final;
  };
}
