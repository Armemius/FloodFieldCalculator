#pragma once

#include <string>
#include <vector>
#include <toml.hpp>
#include <unordered_set>

#include "core/detector/detector.h"
#include "core/filter/filter.h"
#include "image/image_handler.h"

/*
 * If you want to add a new type of filter:
 * - Create special class for it
 * - Add fields specific to this filter to config::Filter
 * - Add parsing logic to it in
 *   pwn::ffc::config::operator>>(toml::basic_value<toml::type_config> table, std::vector<Filter> &filters)
 * - Add if branch to
 *   std::unique_ptr<core::Filter> extractFilter(const Filter &filter)
 */

namespace pwn::ffc::config {
  static const std::unordered_set<std::string> kDetectorTypes = {
    "FLAT",
    "CURVED"
  };

  static const std::unordered_set<std::string> kFilterTypes = {
    "SLAB",
    "DIST",
    "BOWTIE-GAUSS",
    "BOWTIE-PARABOLIC",
    "BOWTIE-CYLINDRICAL"
  };

  static const std::unordered_set<std::string> kCollimatorOrientations = {
    "VERTICAL",
    "HORIZONTAL"
  };

  static const std::unordered_set<std::string> kCollimatorTypes = {
    "LEFT",
    "RIGHT",
    "SYMMETRICAL"
  };

  static const std::unordered_set<std::string> kOutputImageTypes = {
    "TIFF",
    "DICOM",
  };

  static const std::unordered_set<std::string> kTiffPixelDataTypes = {
    "UINT-8",
    "UINT-16",
    "FLOAT-32"
  };

  static const std::unordered_set<std::string> kDicomPixelDataTypes = {
    "UINT-8",
    "UINT-16",
    "FLOAT-32",
    "FLOAT-64"
  };

  struct Resolution {
    double width;
    double height;
  };

  struct Filter {
    std::string type;
    std::string id;
    std::string material;
    double distance;
    double thickness;
    double min_thickness;
    double max_thickness;
    double radius;
    double depth;
    double sigma;
  };

  struct Collimator {
    std::string orientation;
    std::string type;
    double distance;
    double shift;
  };

  struct System {
    std::string output_filename;
    std::string output_type;
    std::string pixel_data;
    bool additional_fields;
    int blur_radius;
    std::string spectrum_table;
    Resolution target_resolution;
    bool logarithmize;
    double scaling_coefficient;
    bool invert;
    bool use_rescale_slope;
  };

  struct Detector {
    std::string type;
    double distance;
    double radius;
    Resolution resolution;
    Resolution size;
  };

  struct Config {
    System system;
    Detector detector;
    std::vector<Filter> filters;
    std::vector<Collimator> collimators;
  };

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param resolution reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, Resolution &resolution);

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param system reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, System &system);

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param detector reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, Detector &detector);

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param filter reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, Filter &filter);

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param filters reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, std::vector<Filter> &filters);

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param collimator reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, Collimator &collimator);

  /**
   * Overloaded operator that fills reference object from toml11 table
   *
   * @param table toml11 table repesentation
   * @param collimators reference object to be filled from given table
   */
  void operator>>(toml::basic_value<toml::type_config> table, std::vector<Collimator> &collimators);

  /**
   * Parses and produces Config instance parsed from provided config file in
   * TOML format
   *
   * @param configPath path to config file in TOML format
   * @return parsed config from file
   * @throws runtime_error if there is errors while parsing file
   */
  Config parseConfig(const std::string &configPath);

  std::unique_ptr<core::Detector> extractDetector(const Detector &detector);

  std::unique_ptr<core::Filter> extractFilter(const Filter &filter);

  std::unique_ptr<image::ImageHandler> extractExportType(const System &system);
}
