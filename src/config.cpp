#include "config.h"

#include <exception>

#include "core/collimator/collimator.h"
#include "core/collimator/horizontal_left_collimator.h"
#include "core/collimator/horizontal_right_collimator.h"
#include "core/collimator/horizontal_symmetrical_collimator.h"
#include "core/collimator/vertical_left_collimator.h"
#include "core/collimator/vertical_right_collimator.h"
#include "core/collimator/vertical_symmetrical_collimator.h"
#include "core/detector/curved_detector.h"
#include "core/detector/flat_detector.h"
#include "core/filter/bowtie_cylindrical_filter.h"
#include "core/filter/bowtie_gauss_filter.h"
#include "core/filter/bowtie_parabolic_filter.h"
#include "core/filter/dist_filter.h"
#include "core/filter/filter.h"
#include "core/filter/slab_filter.h"
#include "core/postprocessing/basic_postprocessor.h"
#include "image/dicom/dicom_float32_handler.h"
#include "image/dicom/dicom_float64_handler.h"
#include "image/dicom/dicom_uint16_handler.h"
#include "image/dicom/dicom_uint8_handler.h"
#include "image/tiff/tiff_float32_handler.h"
#include "image/tiff/tiff_uint16_handler.h"
#include "image/tiff/tiff_uint8_handler.h"

/**
 * Extends to an if branch that will return std::unique_ptr to specified FILTER_CLASS if provided FILTER_TYPE matches
 * current filter.type
 *
 * Only for use in pwn::ffc::config::extractFilter function
 *
 * @param FILTER_TYPE string literal defining filter type
 * @param FILTER_CLASS pwn::ffc::core::Filter class defining which constructor will be used
 */
#define GENERATE_FILTER_CASE(FILTER_TYPE, FILTER_CLASS) \
if (filter.type == FILTER_TYPE) { \
    return std::unique_ptr<core::Filter>( \
      new FILTER_CLASS(filter) \
  ); \
}

/**
 * Extends to an if branch that will return std::unique_ptr to specified COLLIMATOR_CLASS if provided COLLIMATOR_TYPE matches
 * current collimator.type
 *
 * Only for use in pwn::ffc::config::extractCollimator function
 *
 * @param COLLIMATOR_ORIENTATION string literal defining collimator orientation
 * @param COLLIMATOR_TYPE string literal defining collimator type
 * @param COLLIMATOR_CLASS pwn::ffc::core::Collimator class defining which constructor will be used
 */
#define GENERATE_COLLIMATOR_CASE(COLLIMATOR_ORIENTATION, COLLIMATOR_TYPE, COLLIMATOR_CLASS) \
if (collimator.type == COLLIMATOR_TYPE && collimator.orientation == COLLIMATOR_ORIENTATION) { \
  return std::unique_ptr<core::Collimator>( \
    new COLLIMATOR_CLASS(collimator) \
  ); \
}

namespace {
  std::string convertSet2String(const std::unordered_set<std::string> &set) {
    std::string res = "[";
    auto jt = set.begin();
    for (size_t it = 0; it < set.size(); ++it, ++jt) {
      res += *jt;
      if (it < set.size() - 1) {
        res += ", ";
      }
    }
    res += "]";
    return res;
  }
}

namespace pwn::ffc::config {
  void operator>>(toml::basic_value<toml::type_config> table, Resolution &resolution) {
    resolution.height = toml::find<int>(table, "height");
    resolution.width = toml::find<int>(table, "width");
  }

  void operator>>(toml::basic_value<toml::type_config> table, System &system) {
    system.output_filename = toml::find_or<std::string>(table, "output-filename", "flood_field");
    system.output_type = toml::find<std::string>(table, "output-type");
    system.pixel_data = toml::find<std::string>(table, "pixel-data");
    system.blur_radius = toml::find_or<int>(table, "blur-radius", 0);
    system.spectrum_table = toml::find<std::string>(table, "spectrum-table");
    system.invert = toml::find_or<bool>(table, "invert", false);
    system.logarithmize = toml::find_or<bool>(table, "logarithmize", false);
    system.use_rescale_slope = toml::find_or<bool>(table, "use-rescale-slope", false);
    if (system.logarithmize) {
      system.scaling_coefficient = toml::find<double>(table, "scaling-coefficient");
    }
    if (system.invert && (system.pixel_data != "UINT-8" && system.pixel_data != "UINT-16")) {
      const auto error_info = make_error_info("Invertion is only compatible with 'UINT-8' and 'UINT-16' pixel data",
                                              find(table, "inversion"),
                                              "set this to false or change pixel data type");
      throw std::runtime_error(format_error(error_info));
    }
    if (system.use_rescale_slope && (system.pixel_data != "UINT-8" && system.pixel_data != "UINT-16")) {
      const auto error_info = make_error_info("Rescale slope is only compatible with 'UINT-8' and 'UINT-16' pixel data",
                                              find(table, "use-rescale-slope"),
                                              "set this to false or change pixel data type");
      throw std::runtime_error(format_error(error_info));
    }
    find(table, "target-resolution") >> system.target_resolution;
    if (kOutputImageTypes.find(system.output_type) == kOutputImageTypes.end()) {
      const auto error_info = make_error_info("Unsupported output image type: " + system.output_type,
                                              find(table, "output-type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kOutputImageTypes));
      throw std::runtime_error(format_error(error_info));
    }
    if (system.output_type == "TIFF" && kTiffPixelDataTypes.find(system.pixel_data) == kTiffPixelDataTypes.end()) {
      const auto error_info = make_error_info("Unsupported pixel data type: " + system.pixel_data,
                                              find(table, "pixel-data"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kTiffPixelDataTypes));
      throw std::runtime_error(format_error(error_info));
    }
    if (system.output_type == "DICOM" && kDicomPixelDataTypes.find(system.pixel_data) == kDicomPixelDataTypes.
        end()) {
      const auto error_info = make_error_info("Unsupported pixel data type: " + system.pixel_data,
                                              find(table, "pixel-data"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kDicomPixelDataTypes));
      throw std::runtime_error(format_error(error_info));
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, Detector &detector) {
    detector.type = toml::find<std::string>(table, "type");
    detector.distance = toml::find<double>(table, "distance");
    if (detector.type == "CURVED") {
      detector.radius = toml::find<double>(table, "radius");
    }
    find(table, "resolution") >> detector.resolution;
    find(table, "size") >> detector.size;
    if (kDetectorTypes.find(detector.type) == kDetectorTypes.end()) {
      const auto error_info = make_error_info("Unknown detector type: " + detector.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kDetectorTypes));
      throw std::runtime_error(format_error(error_info));
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, Filter &filter) {
    filter.type = toml::find<std::string>(table, "type");
    filter.material = toml::find<std::string>(table, "material");
    if (kFilterTypes.find(filter.type) == kFilterTypes.end()) {
      const auto error_info = make_error_info("Unknown filter type: " + filter.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kFilterTypes));
      throw std::runtime_error(format_error(error_info));
    }
    filter.distance = toml::find<double>(table, "distance");
    if (table.contains("id")) {
      filter.id = toml::find<std::string>(table, "id");
    }
    // Filter parsing logic depending on filter type
    if (filter.type == "SLAB") {
      filter.thickness = toml::find<double>(table, "thickness");
    }
    if (filter.type == "DIST") {
      filter.radius = toml::find<double>(table, "radius");
    }
    if (filter.type == "BOWTIE-PARABOLIC") {
      filter.min_thickness = toml::find<double>(table, "min-thickness");
      filter.max_thickness = toml::find<double>(table, "max-thickness");
      filter.radius = toml::find<double>(table, "radius");
    }
    if (filter.type == "BOWTIE-CYLINDRICAL") {
      filter.thickness = toml::find<double>(table, "thickness");
      filter.radius = toml::find<double>(table, "radius");
    }
    if (filter.type == "BOWTIE-GAUSS") {
      filter.thickness = toml::find<double>(table, "thickness");
      filter.sigma = toml::find<double>(table, "sigma");
      filter.depth = toml::find<double>(table, "depth");
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, std::vector<Filter> &filters) {
    try {
      find(table, "filter");
    } catch (...) {
      return;
    }
    for (const auto &it: find(table, "filter").as_array()) {
      Filter filter;
      it >> filter;
      filters.push_back(filter);
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, Collimator &collimator) {
    collimator.type = toml::find<std::string>(table, "type");
    collimator.orientation = toml::find<std::string>(table, "orientation");
    collimator.distance = toml::find<double>(table, "distance");
    collimator.shift = toml::find<double>(table, "shift");
    if (!kCollimatorOrientations.contains(collimator.orientation)) {
      const auto error_info = make_error_info("Unknown collimator orientation type: " + collimator.type,
                                              find(table, "orientation"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kCollimatorOrientations));
      throw std::runtime_error(format_error(error_info));
    }
    if (collimator.orientation == "HORIZONTAL" && !kHorizontalCollimatorTypes.contains(collimator.type)
    ) {
      const auto error_info = make_error_info("Unknown collimator type: " + collimator.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kHorizontalCollimatorTypes));
      throw std::runtime_error(format_error(error_info));
    }
    if (collimator.orientation == "VERTICAL" && !kVerticalCollimatorTypes.contains(collimator.type)
    ) {
      const auto error_info = make_error_info("Unknown collimator type: " + collimator.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(kVerticalCollimatorTypes));
      throw std::runtime_error(format_error(error_info));
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, std::vector<Collimator> &collimators) {
    try {
      find(table, "collimator");
    } catch (...) {
      return;
    }
    for (const auto &it: find(table, "collimator").as_array()) {
      Collimator collimator;
      it >> collimator;
      collimators.push_back(collimator);
    }
  }

  Config parseConfig(const std::string &configPath) {
    Config config;

    try {
      const auto data = toml::parse(configPath);
      find(data, "system") >> config.system;
      find(data, "detector") >> config.detector;
      data >> config.filters;
      data >> config.collimators;
    } catch (const toml::syntax_error &err) {
      throw std::runtime_error("Syntax error in TOML file: " + std::string(err.what()));
    } catch (const std::out_of_range &err) {
      throw std::runtime_error("Missing parameter in TOML file: " + std::string(err.what()));
    } catch (const std::exception &err) {
      throw std::runtime_error("Error parsing TOML file: " + std::string(err.what()));
    }

    return config;
  }

  std::unique_ptr<core::Detector> extractDetector(const Detector &detector) {
    if (detector.type == "FLAT") {
      return std::unique_ptr<core::Detector>(
        new core::FlatDetector({
                                 static_cast<int>(detector.resolution.width),
                                 static_cast<int>(detector.resolution.height)
                               },
                               {detector.size.width, detector.size.height},
                               detector.distance)
      );
    }
    if (detector.type == "CURVED") {
      return std::unique_ptr<core::Detector>(
        new core::CurvedDetector({
                                   static_cast<int>(detector.resolution.width),
                                   static_cast<int>(detector.resolution.height)
                                 },
                                 {detector.size.width, detector.size.height},
                                 detector.distance,
                                 detector.radius)
      );
    }
    throw std::invalid_argument("Unknown detector type: " + detector.type);
  }

  std::unique_ptr<core::Filter> extractFilter(const Filter &filter) {
    GENERATE_FILTER_CASE("SLAB", core::SlabFilter);
    GENERATE_FILTER_CASE("DIST", core::DistFilter);
    GENERATE_FILTER_CASE("BOWTIE-CYLINDRICAL", core::BowtieCylindricalFilter);
    GENERATE_FILTER_CASE("BOWTIE-PARABOLIC", core::BowtieParabolicFilter);
    GENERATE_FILTER_CASE("BOWTIE-GAUSS", core::BowtieGaussFilter);
    throw std::invalid_argument("Unknown filter type: " + filter.type);
  }

  std::unique_ptr<core::Collimator> extractCollimator(const Collimator &collimator) {
    GENERATE_COLLIMATOR_CASE("VERTICAL", "LEFT", core::VerticalLeftCollimator);
    GENERATE_COLLIMATOR_CASE("VERTICAL", "RIGHT", core::VerticalRightCollimator);
    GENERATE_COLLIMATOR_CASE("VERTICAL", "SYMMETRICAL", core::VerticalSymmetricalCollimator);
    GENERATE_COLLIMATOR_CASE("HORIZONTAL", "TOP", core::HorizontalTopCollimator);
    GENERATE_COLLIMATOR_CASE("HORIZONTAL", "BOTTOM", core::HorizontalBottomCollimator);
    GENERATE_COLLIMATOR_CASE("HORIZONTAL", "SYMMETRICAL", core::HorizontalSymmetricalCollimator);
    throw std::invalid_argument("Unknown collimator type: " + collimator.type);
  }

  std::unique_ptr<image::ImageHandler> extractExportType(const System &system) {
    std::unique_ptr<core::Postprocessor> postprocessor(new core::BasicPostprocessor(system.blur_radius,
                                                                                    system.target_resolution.width,
                                                                                    system.target_resolution.height,
                                                                                    system.invert
    ));
    if (system.output_type == "TIFF") {
      if (system.pixel_data == "UINT-8") {
        return std::unique_ptr<image::ImageHandler>(new image::TiffUint8Handler(std::move(postprocessor)));
      }
      if (system.pixel_data == "UINT-16") {
        return std::unique_ptr<image::ImageHandler>(new image::TiffUint16Handler(std::move(postprocessor)));
      }
      if (system.pixel_data == "FLOAT-32") {
        return std::unique_ptr<image::ImageHandler>(new image::TiffFloat32Handler());
      }
      throw std::invalid_argument("Unknown tiff pixel data: " + system.pixel_data);
    }
    if (system.output_type == "DICOM") {
      if (system.pixel_data == "UINT-8") {
        return std::unique_ptr<image::ImageHandler>(
          new image::DicomUint8Handler(system.use_rescale_slope, std::move(postprocessor)));
      }
      if (system.pixel_data == "UINT-16") {
        return std::unique_ptr<image::ImageHandler>(
          new image::DicomUint16Handler(system.use_rescale_slope, std::move(postprocessor)));
      }
      if (system.pixel_data == "FLOAT-32") {
        return std::unique_ptr<image::ImageHandler>(new image::DicomFloat32Handler());
      }
      if (system.pixel_data == "FLOAT-64") {
        return std::unique_ptr<image::ImageHandler>(new image::DicomFloat64Handler());
      }
      throw std::invalid_argument("Unknown dicom pixel data: " + system.pixel_data);
    }
    throw std::invalid_argument("Unknown output image type: " + system.output_type);
  }
}
