#pragma once

#include <string>
#include <vector>
#include <toml.hpp>
#include <unordered_set>

namespace pwn::ffc::config {
  static const std::unordered_set<std::string> DETECTOR_TYPES = {
    "FLAT",
    "CURVED"
  };

  static const std::unordered_set<std::string> FILTER_TYPES = {
    "FLAT",
    "DIST",
    "BOWTIE-GAUSS",
    "BOWTIE-CURVED",
    "BOWTIE-CYLINDRICAL"
  };

  static const std::unordered_set<std::string> COLLIMATOR_ORIENTATIONS = {
    "VERTICAL",
    "HORIZONTAL"
  };

  static const std::unordered_set<std::string> COLLIMATOR_TYPES = {
    "LEFT",
    "RIGHT",
    "SYMMETRICAL"
  };

  static const std::unordered_set<std::string> OUTPUT_IMAGE_TYPES = {
    "TIFF",
    "DICOM",
  };

  static const std::unordered_set<std::string> TIFF_PIXEL_DATA_TYPES = {
    "UINT-8",
    "UINT-16",
    "FLOAT-32"
  };

  static const std::unordered_set<std::string> DICOM_PIXEL_DATA_TYPES = {
    "UINT-8",
    "UINT-16",
    "FLOAT-32",
    "FLOAT-64"
  };

  struct Resolution {
    int width;
    int height;
  };

  struct Filter {
    std::string type;
    double thickness;
    double distance;
    std::string material;
  };

  struct Collimator {
    std::string orientation;
    std::string type;
    double distance;
    double shift;
  };

  struct System {
    std::string outputFilename;
    std::string outputType;
    std::string pixelData;
    int blurRadius;
    double scalingCoefficient;
    std::string spectrumTable;
    Resolution targetResolution;
    bool invert;
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
}
