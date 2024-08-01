#include "image/dicom/dicom_uint8_handler.h"

#include <dcmdata/dcdeftag.h>

namespace pwn::ffc::image {
  void DicomUint8Handler::setPixelData_(cv::Mat &mat, DcmDataset *dataset) {
    mat.convertTo(mat, CV_8UC1);
    dataset->putAndInsertUint16(DCM_BitsAllocated, 8);
    dataset->putAndInsertUint16(DCM_BitsStored, 8);
    dataset->putAndInsertUint16(DCM_HighBit, 7);

    dataset->putAndInsertUint8Array(DCM_PixelData, mat.data, mat.total());
  }
}
