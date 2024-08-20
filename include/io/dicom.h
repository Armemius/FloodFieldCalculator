#pragma once

#include <opencv2/opencv.hpp>
#include <dcmtk/dcmdata/dcfilefo.h>

#define DCM_DetectorFocalCenterAxialPosition DcmTag(0x7031, 0x1002, EVR_DS)
#define DCM_DarkFieldCorrectionFlag DcmTag(0x7039, 0x1005, EVR_CS)
#define DCM_FlatFieldCorrectionFlag DcmTag(0x7039, 0x1006, EVR_CS)
#define DCM_LogFlag DcmTag(0x7031, 0x1009, EVR_CS)

typedef std::pair<std::string, int> instance_file;

namespace pwn::io {
    struct DcmImage {
        cv::Mat mat;
        DcmFileFormat format;
    };

    DcmImage readDicom(const std::string &path);

    std::vector<DcmImage> readDicoms(const std::string &path);

    std::vector<DcmImage> readDicomsOrdered(const std::string &path);



    std::vector<std::string> sort_input_files(const std::vector<std::string> &input_files);
}
