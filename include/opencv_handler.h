#pragma once
#include "frame.h"
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/opencv.hpp>

// Disables logging for opencv
void disable_logging();

// Sets logging level for opencv
void set_log_level(cv::utils::logging::LogLevel);

cv::Mat apply_blur(const cv::Mat &image, int radius);

void open_frame(const ffc::Frame &frame);

void export_frame(const ffc::Frame &frame, const std::string &filename,
                  int target_width, int target_height, int blur_radius, const std::string &type, double K, bool invert);

void export_additional_frame(const ffc::Frame &frame, const std::string &filename,
                  int target_width, int target_height, const std::string& type);

cv::Mat rescale(const cv::Mat &image, int width, int height);
