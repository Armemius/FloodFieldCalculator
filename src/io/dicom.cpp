#include "io/dicom.h"

#include <filesystem>
#include <bits/fs_dir.h>
#include <dcmdata/dcdeftag.h>
#include <dcmdata/dcfilefo.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include "io/fs.h"

pwn::io::DcmImage readDicomFromImage(const std::string &path) {
    spdlog::debug("Reading DICOM .. {}", path);
    DcmFileFormat dcm_file_format;
    if (dcm_file_format.loadFile(path.c_str()).bad()) {
        throw std::runtime_error(fmt::format("Unable to open DICOM document: {}", path));
    }
    DcmDataset *dataset = dcm_file_format.getDataset();
    uint16_t rows;
    uint16_t cols;
    dataset->findAndGetUint16(DCM_Rows, rows);
    dataset->findAndGetUint16(DCM_Columns, cols);
    uint16_t bits_allocated;
    uint16_t bits_stored;
    uint16_t high_bit;
    dataset->findAndGetUint16(DCM_BitsAllocated, bits_allocated);
    dataset->findAndGetUint16(DCM_BitsStored, bits_stored);
    dataset->findAndGetUint16(DCM_HighBit, high_bit);

    spdlog::debug("DICOM image has {}x{} dimensions", cols, rows);
    cv::Mat img;
    if (DcmElement *element; dataset->findAndGetElement(DCM_PixelData, element).good()) {
        if (bits_allocated == 16) {
            spdlog::debug("Pixel data identified as UINT-16");
            uint16_t *pixel_data;
            element->getUint16Array(pixel_data);
            img = cv::Mat(rows, cols, CV_16UC1, pixel_data);
        } else if (bits_allocated == 8) {
            spdlog::debug("Pixel data identified as UINT-8");
            Uint8 *pixel_data;
            element->getUint8Array(pixel_data);
            img = cv::Mat(rows, cols, CV_8UC1, pixel_data);
        }
    } else if (dataset->findAndGetElement(DCM_FloatPixelData, element).good()) {
        spdlog::debug("Pixel data identified as FLOAT-32");
        float *pixel_data;
        element->getFloat32Array(pixel_data);
        img = cv::Mat(rows, cols, CV_32FC1, pixel_data);
    } else if (dataset->findAndGetElement(DCM_DoubleFloatPixelData, element).good()) {
        spdlog::debug("Pixel data identified as FLOAT-64");
        double *pixel_data;
        element->getFloat64Array(pixel_data);
        img = cv::Mat(rows, cols, CV_64FC1, pixel_data);
    } else {
        throw std::runtime_error(fmt::format("Unsupported pixel data type in DICOM file {}", path));
    }

    if (img.type() != CV_64FC1) {
        img.convertTo(img, CV_64FC1);
    }

    // Adjust additional tags
    DcmElement *element;
    if (dataset->findAndGetElement(DCM_RescaleSlope, element).good()) {
        if (double rescale_slope; element->getFloat64(rescale_slope).good()) {
            spdlog::debug("Rescale slope tag found (RESCALE_SLOPE={})", rescale_slope);
            img *= rescale_slope;
        } else {
            spdlog::error("Rescale slope tag found, but contain broken data");
        }
    }
    if (dataset->findAndGetElement(DCM_RescaleIntercept, element).good()) {
        if (double rescale_intercept; element->getFloat64(rescale_intercept).good()) {
            spdlog::debug("Rescale intercept tag found (RESCALE_INTERCEPT={})", rescale_intercept);
            img += rescale_intercept;
        } else {
            spdlog::error("Rescale intercept tag found, but contain broken data");
        }
    }
    if (dataset->findAndGetElement(DCM_LogFlag, element).good()) {
        if (char *log_flag; element->getString(log_flag).good()) {
            spdlog::debug("Log flag tag found (LOG_FLAG={})", log_flag);
            if (std::string(log_flag) == "YES") {
                cv::exp(-img, img);
                img *= 64000;
            }
        } else {
            spdlog::error("Log flag tag found, but contain broken data");
        }
    }

    return {img.clone(), dcm_file_format};
}

pwn::io::DcmImage pwn::io::readDicom(const std::string &path) {
    if (file_exists(path)) {
        return readDicomFromImage(path);
    }
    if (!directory_exists(path)) {
        throw std::runtime_error(fmt::format("Provided dicom {} does not exist", path));
    }
    auto images = readDicoms(path);
    if (images.empty()) {
        throw std::runtime_error(fmt::format("Provided dicom folder {} is empty", path));
    }
    cv::Mat mean = cv::Mat::zeros(images.front().mat.rows, images.front().mat.cols, CV_64FC1);
    for (const auto &[mat, format]: images) {
        mean += mat;
    }
    mean /= static_cast<double>(images.size());
    return {mean, images.front().format};
}

std::vector<pwn::io::DcmImage> pwn::io::readDicoms(const std::string &path) {
    std::vector<std::string> files;

    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            if (entry.path().extension().string() == ".dcm") {
                files.push_back(entry.path().string());
            }
        }
    }

    std::vector<DcmImage> dcm_images;
    for (const auto &image_path: files) {
        dcm_images.push_back(readDicom(image_path));
    }

    return dcm_images;
}

std::vector<pwn::io::DcmImage> pwn::io::readDicomsOrdered(const std::string &path) {
    std::vector<std::string> files;

    for (const auto &entry: std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            if (entry.path().extension().string() == ".dcm") {
                files.push_back(entry.path().string());
            }
        }
    }

    files = sort_input_files(files);

    std::vector<DcmImage> dcm_images;
    for (const auto &image_path: files) {
        dcm_images.push_back(readDicom(image_path));
    }

    return dcm_images;
}

std::vector<std::string> pwn::io::sort_input_files(const std::vector<std::string> &input_files) {
    std::vector<instance_file> files;

    size_t index = 0;
    for (const auto &dicom_path: input_files) {
        ++index;

        DcmFileFormat file_format;
        OFCondition status = file_format.loadFile(dicom_path);
        if (!status.good()) {
            throw std::runtime_error("Failed to load DICOM file: " + dicom_path);
        }

        DcmDataset *dataset = file_format.getDataset();
        DcmTagKey instance_number(0x0020, 0x0013);
        OFString instance_string;
        status = dataset->findAndGetOFString(instance_number, instance_string);
        if (!status.good()) {
            throw std::runtime_error("Failed to retrieve Exposure attribute (0018,1152).");
        }
        int instance = std::stoi(instance_string);
        files.emplace_back(dicom_path, instance);
    }

    std::sort(files.begin(), files.end(), [](instance_file &a, instance_file &b) {
        return b.second > a.second;
    });

    std::vector<std::string> res;
    for (const auto &[fst, snd]: files) {
        res.push_back(fst);
    }

    return res;
}
