#include "image/dicom/dicom_float64_handler.h"

#include <dcmdata/dcdeftag.h>

namespace pwn::ffc::image {
  void DicomFloat64Handler::setPixelData_(cv::Mat &mat, DcmDataset *dataset) {
    mat.convertTo(mat, CV_64FC1);

    dataset->putAndInsertUint16(DCM_BitsAllocated, 64);
    dataset->putAndInsertUint16(DCM_BitsStored, 64);
    dataset->putAndInsertUint16(DCM_HighBit, 63);

    dataset->putAndInsertFloat64Array(DCM_DoubleFloatPixelData, reinterpret_cast<double *>(mat.data), mat.total());
  }
}
