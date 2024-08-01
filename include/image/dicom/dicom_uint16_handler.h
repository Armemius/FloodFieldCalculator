#pragma once

#include "image/image_handler.h"

#include "dicom_handler.h"

namespace pwn::ffc::image {
  class DicomUint16Handler : public DicomHandler {
    bool m_rescale_slope;

  protected:
    void setPixelData_(cv::Mat &mat, DcmDataset *dataset) override;

  public:
    explicit DicomUint16Handler(bool use_rescale_slope);
  };
}
