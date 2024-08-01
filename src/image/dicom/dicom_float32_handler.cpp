#include "image/dicom/dicom_float32_handler.h"

#include <dcmdata/dcdeftag.h>

namespace pwn::ffc::image {
  void DicomFloat32Handler::setPixelData_(cv::Mat &mat, DcmDataset *dataset) {
    mat.convertTo(mat, CV_32FC1);

    dataset->putAndInsertUint16(DCM_BitsAllocated, 32);
    dataset->putAndInsertUint16(DCM_BitsStored, 32);
    dataset->putAndInsertUint16(DCM_HighBit, 31);

    dataset->putAndInsertFloat32Array(DCM_FloatPixelData, reinterpret_cast<float *>(mat.data), mat.total());
  }
}
