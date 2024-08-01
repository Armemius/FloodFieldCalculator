#pragma once

#include "image/image_handler.h"

#include "dicom_handler.h"

namespace pwn::ffc::image {
  class DicomUint8Handler : public DicomHandler {
  private:
    bool m_rescale_slope;
  protected:
    void setPixelData_(cv::Mat &mat, DcmDataset *dataset) override;
  public:
    DicomUint8Handler(bool use_rescale_slope);
  };
}
