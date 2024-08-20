#include "image/dicom/dicom_uint16_handler.h"

#include <dcmdata/dcdeftag.h>

namespace pwn::ffc::image {
  void DicomUint16Handler::setPixelData_(cv::Mat &mat, DcmDataset *dataset) {
    if (!m_rescale_slope) {
      threshold(mat, mat, UINT16_MAX, UINT16_MAX, cv::THRESH_TRUNC);
    } else {
      double min;
      double max;
      minMaxLoc(mat, &min, &max);
      const double rescale_slope = 0.0001;  // max / UINT16_MAX;
      constexpr double kRescaleIntercept = 0;
      mat *= 1 / rescale_slope;
      dataset->putAndInsertString(DCM_RescaleSlope, std::to_string(rescale_slope).c_str());
      dataset->putAndInsertString(DCM_RescaleIntercept, std::to_string(kRescaleIntercept).c_str());
    }
    mat.convertTo(mat, CV_16UC1);
    m_postprocessor->process(mat);

    dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
    dataset->putAndInsertUint16(DCM_BitsStored, 16);
    dataset->putAndInsertUint16(DCM_HighBit, 15);

    dataset->putAndInsertUint16Array(DCM_PixelData, reinterpret_cast<uint16_t *>(mat.data), mat.total());
  }

  DicomUint16Handler::DicomUint16Handler(const bool use_rescale_slope,
                                         std::unique_ptr<core::Postprocessor> postprocessor)
    : m_rescale_slope(use_rescale_slope), m_postprocessor(std::move(postprocessor)) {
  }
}
