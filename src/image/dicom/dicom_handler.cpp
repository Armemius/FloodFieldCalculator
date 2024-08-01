#include "image/dicom/dicom_handler.h"

#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <spdlog/spdlog.h>

namespace pwn::ffc::image {
  void DicomHandler::outputImage(const cv::Mat &mat, const std::string &filename) {
    std::string dicomFilename = filename;
    if (filename.substr(filename.find_last_of('.') + 1) != "dcm") {
      dicomFilename += ".dcm";
    }
    DcmFileFormat fileformat;
    DcmDataset *dataset = fileformat.getDataset();
    dataset->putAndInsertString(DCM_PatientName, "FloodFieldCalculator");
    dataset->putAndInsertString(DCM_PatientID, "1337");
    const auto uid = std::string("1337");
    dataset->putAndInsertString(DCM_StudyInstanceUID, uid.data());
    dataset->putAndInsertString(DCM_SeriesInstanceUID, uid.data());
    dataset->putAndInsertString(DCM_SOPInstanceUID, uid.data());
    dataset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
    dataset->putAndInsertUint16(DCM_Rows, mat.rows);
    dataset->putAndInsertUint16(DCM_Columns, mat.cols);
    dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
    dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
    dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
    cv::Mat convertedImage;
    mat.copyTo(convertedImage);
    setPixelData_(convertedImage, dataset);
    if (const OFCondition status = fileformat.saveFile(dicomFilename.c_str(),
                                                        EXS_LittleEndianExplicit); status.bad()) {
      throw std::runtime_error("cannot write DICOM file (" + std::string(status.text()) + ")");
    }
  }
}
