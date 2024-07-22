#include "opencv_handler.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcistrmf.h"
#include "dcmtk/dcmdata/dcuid.h"

void disable_logging() {
    constexpr auto LOG_SILENT = cv::utils::logging::LogLevel::LOG_LEVEL_SILENT;
    setLogLevel(LOG_SILENT);
}

void set_log_level(const cv::utils::logging::LogLevel level) {
    setLogLevel(level);
}

cv::Mat apply_blur(const cv::Mat &image, int radius) {
    cv::Mat res;

    if (radius <= 0) {
        radius = 1;
    }
    if (radius % 2 == 0) {
        radius += 1;
    }
    GaussianBlur(image, res, cv::Size(radius, radius), 0, 0);

    return res;
}

cv::Mat convert_to_float(const cv::Mat &image) {
    cv::Mat res;
    image.convertTo(res, CV_32FC1);
    return res;
}

cv::Mat rescale(const cv::Mat &image, const int width, const int height) {
    cv::Mat res;
    resize(image, res, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);
    return res;
}

void open_frame(const ffc::Frame &frame) {
    const double K = 1.0;

    std::vector<uint8_t> pixel_data(frame.get_height() * frame.get_width());

    for (size_t it = 0; it < pixel_data.size(); ++it) {
        pixel_data[it] =
                static_cast<uint8_t>(-K * log(frame.get_data()->at(it)));
    }

    cv::Mat image(frame.get_height(), frame.get_width(), CV_8UC1,
                  pixel_data.data());

    namedWindow("Flood field", cv::WINDOW_AUTOSIZE);
    imshow("Flood field", rescale(image, 32, 896));

    cv::waitKey(0);
}

void export_frame(const ffc::Frame &frame, const std::string &filename,
                  int target_width, int target_height, int blur_radius, const std::string &type, const double K = 1.0,
                  const bool invert = false) {
    if (type == "TIFF") {
        std::vector<uint8_t> pixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = -K * log(frame.get_data()->at(it));
            if (value > 255) {
                value = 255;
            }
            if (value < 0) {
                value = 0;
            }
            if (!invert) {
                pixel_data[it] =
                        static_cast<uint8_t>(255 - value);
            } else {
                pixel_data[it] =
                        static_cast<uint8_t>(value);
            }

        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_8UC1,
                            pixel_data.data());
        imwrite(filename + ".tiff", rescale(apply_blur(image, blur_radius), target_width,
                                            target_height));
    }
    if (type == "TIFF-16") {
        std::vector<uint16_t> pixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = -K * log(frame.get_data()->at(it)) * 256.0;
            if (value > 65535) {
                value = 65535;
            }
            if (value < 0) {
                value = 0;
            }
            if (!invert) {
                pixel_data[it] =
                        static_cast<uint16_t>(65535 - value);
            } else {
                pixel_data[it] =
                        static_cast<uint16_t>(value);
            }
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_16UC1,
                            pixel_data.data());
        imwrite(filename + ".tiff", rescale(apply_blur(image, blur_radius), target_width,
                                            target_height));
    }
    if (type == "DICOM") {
        std::vector<uint16_t> pixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = -K * log(frame.get_data()->at(it)) * 256.0;
            if (value > 65535) {
                value = 65535;
            }
            if (value < 0) {
                value = 0;
            }
            if (!invert) {
                pixel_data[it] =
                        static_cast<uint16_t>(65535 - value);
            } else {
                pixel_data[it] =
                        static_cast<uint16_t>(value);
            }
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_16UC1,
                            pixel_data.data());

        DcmFileFormat fileFormat;
        DcmDataset *dataset = fileFormat.getDataset();
        dataset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
        dataset->putAndInsertString(DCM_StudyID, "FloodField_Calculated");
        dataset->putAndInsertString(DCM_SeriesNumber, "1");
        dataset->putAndInsertString(DCM_InstanceNumber, "1");
        dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY");
        dataset->putAndInsertString(DCM_Modality, "CT-PD");
        dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        dataset->putAndInsertUint16(DCM_Rows, target_height);
        dataset->putAndInsertUint16(DCM_Columns, target_width);
        dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
        dataset->putAndInsertUint16(DCM_BitsStored, 16);
        dataset->putAndInsertUint16(DCM_HighBit, 15);
        dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
        DcmElement *pixelData = new DcmPixelData(DCM_PixelData);
        dataset->insert(pixelData);
        pixelData->putUint16Array(reinterpret_cast<const Uint16 *>(rescale(apply_blur(image, blur_radius), target_width,
                                                                           target_height).data),
                                  target_height * target_width);
        fileFormat.saveFile((filename + ".dcm").c_str(), EXS_LittleEndianExplicit);
    }
    if (type == "DICOM-FLOAT") {
        std::vector<uint16_t> pixel_data(frame.get_height() * frame.get_width());
        std::vector<Float32> fpixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = -K * log(frame.get_data()->at(it)) * 256.0;
            if (value > 65535) {
                value = 65535;
            }
            if (value < 0) {
                value = 0;
            }
            pixel_data[it] =
                    static_cast<uint16_t>(65535 - value);
            fpixel_data[it] = static_cast<Float32>((65535 - value) / 65535);
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_16UC1,
                            pixel_data.data());

        DcmFileFormat fileFormat;
        DcmDataset *dataset = fileFormat.getDataset();
        dataset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
        dataset->putAndInsertString(DCM_StudyID, "FloodField_Calculated");
        dataset->putAndInsertString(DCM_SeriesNumber, "1");
        dataset->putAndInsertString(DCM_InstanceNumber, "1");
        dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY");
        dataset->putAndInsertString(DCM_Modality, "CT-PD");
        dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        dataset->putAndInsertUint16(DCM_Rows, target_height);
        dataset->putAndInsertUint16(DCM_Columns, target_width);
        dataset->putAndInsertUint16(DCM_BitsAllocated, 32);
        dataset->putAndInsertUint16(DCM_BitsStored, 32);
        dataset->putAndInsertUint16(DCM_HighBit, 31);
        dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
        // DcmElement *pixelData = new DcmPixelData(DCM_PixelData);
        // dataset->insert(pixelData);
        // pixelData->putUint16Array(reinterpret_cast<const Uint16 *>(rescale(apply_blur(image, blur_radius), target_width,
        //                                                                    target_height).data),
        //                           target_height * target_width);

        const cv::Mat fimage(frame.get_height(), frame.get_width(), CV_32FC1,
                             fpixel_data.data());

        const auto *ddata = reinterpret_cast<const Float32 *>(rescale(apply_blur(fimage, blur_radius), target_width,
                                                                      target_height).data);
        auto res = dataset->putAndInsertFloat32Array(DCM_FloatPixelData, ddata, target_height * target_width);
        fileFormat.saveFile((filename + ".dcm").c_str(), EXS_LittleEndianExplicit);
    }
}

void export_additional_frame(const ffc::Frame &frame, const std::string &filename,
                             int target_width, int target_height, const std::string &type) {
    if (type == "TIFF") {
        std::vector<uint8_t> pixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = frame.get_data()->at(it);
            if (value > 255) {
                value = 255;
            }
            if (value < 0) {
                value = 0;
            }
            pixel_data[it] = value;
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_8UC1,
                            pixel_data.data());
        imwrite(filename + ".tiff", rescale(image, target_width,
                                            target_height));
    }
    if (type == "TIFF-16") {
        std::vector<uint16_t> pixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = frame.get_data()->at(it);
            if (value > 65535) {
                value = 65535;
            }
            if (value < 0) {
                value = 0;
            }
            pixel_data[it] = static_cast<uint16_t>(value);
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_16UC1,
                            pixel_data.data());
        imwrite(filename + ".tiff", rescale(image, target_width,
                                            target_height));
    }
    if (type == "DICOM") {
        std::vector<uint16_t> pixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < pixel_data.size(); ++it) {
            double value = frame.get_data()->at(it);
            if (value > 65535) {
                value = 65535;
            }
            if (value < 0) {
                value = 0;
            }
            pixel_data[it] = static_cast<uint16_t>(value);
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_16UC1,
                            pixel_data.data());

        DcmFileFormat fileFormat;
        DcmDataset *dataset = fileFormat.getDataset();
        dataset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
        dataset->putAndInsertString(DCM_StudyID, "FloodField_Calculated");
        dataset->putAndInsertString(DCM_SeriesNumber, "1");
        dataset->putAndInsertString(DCM_InstanceNumber, "1");
        dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY");
        dataset->putAndInsertString(DCM_Modality, "CT-PD");
        dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        dataset->putAndInsertUint16(DCM_Rows, target_height);
        dataset->putAndInsertUint16(DCM_Columns, target_width);
        dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
        dataset->putAndInsertUint16(DCM_BitsStored, 16);
        dataset->putAndInsertUint16(DCM_HighBit, 15);
        dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);
        DcmElement *pixelData = new DcmPixelData(DCM_PixelData);
        dataset->insert(pixelData);
        pixelData->putUint16Array(reinterpret_cast<const Uint16 *>(rescale(image, target_width,
                                                                           target_height).data),
                                  target_height * target_width);
        fileFormat.saveFile((filename + ".dcm").c_str(), EXS_LittleEndianExplicit);
    }
    if (type == "DICOM-FLOAT") {
        std::vector<Float32> fpixel_data(frame.get_height() * frame.get_width());
        for (size_t it = 0; it < fpixel_data.size(); ++it) {
            double value = frame.get_data()->at(it);
            if (value > 65535) {
                value = 65535;
            }
            if (value < 0) {
                value = 0;
            }
            fpixel_data[it] = static_cast<Float32>(value);
        }
        const cv::Mat image(frame.get_height(), frame.get_width(), CV_16UC1,
                            fpixel_data.data());

        DcmFileFormat fileFormat;
        DcmDataset *dataset = fileFormat.getDataset();
        dataset->putAndInsertString(DCM_SOPClassUID, UID_SecondaryCaptureImageStorage);
        dataset->putAndInsertString(DCM_StudyID, "FloodField_Calculated");
        dataset->putAndInsertString(DCM_SeriesNumber, "1");
        dataset->putAndInsertString(DCM_InstanceNumber, "1");
        dataset->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY");
        dataset->putAndInsertString(DCM_Modality, "CT-PD");
        dataset->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        dataset->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        dataset->putAndInsertUint16(DCM_Rows, target_height);
        dataset->putAndInsertUint16(DCM_Columns, target_width);
        dataset->putAndInsertUint16(DCM_BitsAllocated, 32);
        dataset->putAndInsertUint16(DCM_BitsStored, 32);
        dataset->putAndInsertUint16(DCM_HighBit, 31);
        dataset->putAndInsertUint16(DCM_PixelRepresentation, 0);

        const cv::Mat fimage(frame.get_height(), frame.get_width(), CV_32FC1,
                             fpixel_data.data());

        const auto *ddata = reinterpret_cast<const Float32 *>(rescale(fimage, target_width,
                                                                      target_height).data);
        auto res = dataset->putAndInsertFloat32Array(DCM_FloatPixelData, ddata, target_height * target_width);
        fileFormat.saveFile((filename + ".dcm").c_str(), EXS_LittleEndianExplicit);
    }
}
