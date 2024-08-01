#include "image/dicom/dicom_uint16_handler.h"

#include <dcmdata/dcdeftag.h>

namespace pwn::ffc::image {
  void DicomUint16Handler::setPixelData_(cv::Mat &mat, DcmDataset *dataset) {
    mat.convertTo(mat, CV_16UC1);

    dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
    dataset->putAndInsertUint16(DCM_BitsStored, 16);
    dataset->putAndInsertUint16(DCM_HighBit, 15);

    dataset->putAndInsertUint16Array(DCM_PixelData, reinterpret_cast<uint16_t *>(mat.data), mat.total());
  }
}
