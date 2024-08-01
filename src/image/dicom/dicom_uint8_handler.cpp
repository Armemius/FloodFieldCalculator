#include "image/dicom/dicom_uint8_handler.h"

#include <dcmdata/dcdeftag.h>

namespace pwn::ffc::image {
  void DicomUint8Handler::setPixelData_(cv::Mat &mat, DcmDataset *dataset) {
    if (!m_rescale_slope) {
      threshold(mat, mat, UINT8_MAX, UINT8_MAX, cv::THRESH_TRUNC);
    } else {
      double min;
      double max;
      minMaxLoc(mat, &min, &max);
      const double rescale_slope = max / UINT8_MAX;
      const double rescale_intercept = min;
      mat *= 1 / rescale_slope;
      dataset->putAndInsertString(DCM_RescaleSlope, std::to_string(rescale_slope).c_str());
      dataset->putAndInsertString(DCM_RescaleIntercept, std::to_string(rescale_intercept).c_str());
    }
    mat.convertTo(mat, CV_8UC1);
    dataset->putAndInsertUint16(DCM_BitsAllocated, 8);
    dataset->putAndInsertUint16(DCM_BitsStored, 8);
    dataset->putAndInsertUint16(DCM_HighBit, 7);

    dataset->putAndInsertUint8Array(DCM_PixelData, mat.data, mat.total());
  }


  DicomUint8Handler::DicomUint8Handler(const bool use_rescale_slope): m_rescale_slope(use_rescale_slope) {
  }
}
