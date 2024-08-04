#pragma once

#include "image/image_handler.h"

#include "dicom_handler.h"
#include "core/postprocessing/postprocessor.h"

namespace pwn::ffc::image {
  class DicomUint16Handler final : public DicomHandler {
    bool m_rescale_slope;
    std::unique_ptr<core::Postprocessor> m_postprocessor;

  protected:
    void setPixelData_(cv::Mat &mat, DcmDataset *dataset) override;

  public:
    explicit DicomUint16Handler(bool use_rescale_slope, std::unique_ptr<core::Postprocessor> postprocessor);
  };
}
