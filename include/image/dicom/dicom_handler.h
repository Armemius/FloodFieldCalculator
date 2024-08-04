#pragma once

#include "image/image_handler.h"

#include <dcmtk/dcmdata/dcfilefo.h>

namespace pwn::ffc::image {
  /// Class that handles exporting OpenCV Mat objects to DICOM images and properly handling DICOM tags
  class DicomHandler : public ImageHandler {
  protected:
    /**
     * Puts OpenCV mat pixeldata inside Dicom dataset
     *
     * @param mat reference to Mat to export
     * @param dataset pointer to Dicom dataset
     */
    virtual void setPixelData_(cv::Mat &mat, DcmDataset *dataset) = 0;

  public:
    /**
     * Exports OpenCV Mat objects to DICOM format and stores resulting image inside filesystem
     *
     * @param mat reference to Mat to export
     * @param filename name of exported file (without extension)
     */
    void outputImage(const cv::Mat &mat, const std::string &filename) final;
  };
}
