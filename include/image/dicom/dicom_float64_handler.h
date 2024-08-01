#pragma once

#include "image/image_handler.h"

#include "dicom_handler.h"

namespace pwn::ffc::image {
  class DicomFloat64Handler : public DicomHandler {
  protected:
    void setPixelData_(cv::Mat &mat, DcmDataset *dataset) override;
  };
}
