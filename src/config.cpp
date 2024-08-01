#include "config.h"

#include <exception>

#include "core/detector/curved_detector.h"
#include "core/detector/flat_detector.h"
#include "core/filter/filter.h"
#include "core/filter/slab_filter.h"
#include "image/dicom/dicom_float32_handler.h"
#include "image/dicom/dicom_float64_handler.h"
#include "image/dicom/dicom_uint16_handler.h"
#include "image/dicom/dicom_uint8_handler.h"
#include "image/tiff/tiff_uint16_handler.h"
#include "image/tiff/tiff_uint8_handler.h"

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
    if (OUTPUT_IMAGE_TYPES.find(system.output_type) == OUTPUT_IMAGE_TYPES.end()) {
      const auto error_info = make_error_info("Unsupported output image type: " + system.output_type,
                                              find(table, "output-type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(OUTPUT_IMAGE_TYPES));
      throw std::runtime_error(format_error(error_info));
    }
    if (system.output_type == "TIFF" && TIFF_PIXEL_DATA_TYPES.find(system.pixel_data) == TIFF_PIXEL_DATA_TYPES.end()) {
      const auto error_info = make_error_info("Unsupported pixel data type: " + system.pixel_data,
                                              find(table, "pixel-data"),
                                              "at this row",
                                              "possible values: " + convertSet2String(TIFF_PIXEL_DATA_TYPES));
      throw std::runtime_error(format_error(error_info));
    }
    if (system.output_type == "DICOM" && DICOM_PIXEL_DATA_TYPES.find(system.pixel_data) == DICOM_PIXEL_DATA_TYPES.
        end()) {
      const auto error_info = make_error_info("Unsupported pixel data type: " + system.pixel_data,
                                              find(table, "pixel-data"),
                                              "at this row",
                                              "possible values: " + convertSet2String(DICOM_PIXEL_DATA_TYPES));
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
    if (DETECTOR_TYPES.find(detector.type) == DETECTOR_TYPES.end()) {
      const auto error_info = make_error_info("Unknown detector type: " + detector.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(DETECTOR_TYPES));
      throw std::runtime_error(format_error(error_info));
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, Filter &filter) {
    filter.type = toml::find<std::string>(table, "type");
    filter.material = toml::find<std::string>(table, "material");
    if (FILTER_TYPES.find(filter.type) == FILTER_TYPES.end()) {
      const auto error_info = make_error_info("Unknown filter type: " + filter.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(FILTER_TYPES));
      throw std::runtime_error(format_error(error_info));
    }
    filter.distance = toml::find<double>(table, "distance");
    if (filter.type == "SLAB") {
      filter.thickness = toml::find<double>(table, "thickness");
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, std::vector<Filter> &filters) {
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
    if (COLLIMATOR_TYPES.find(collimator.type) == COLLIMATOR_TYPES.end()) {
      const auto error_info = make_error_info("Unknown collimator type: " + collimator.type,
                                              find(table, "type"),
                                              "at this row",
                                              "possible values: " + convertSet2String(COLLIMATOR_TYPES));
      throw std::runtime_error(format_error(error_info));
    }
    if (COLLIMATOR_ORIENTATIONS.find(collimator.orientation) == COLLIMATOR_ORIENTATIONS.end()) {
      const auto error_info = make_error_info("Unknown collimator orientation type: " + collimator.type,
                                              find(table, "orientation"),
                                              "at this row",
                                              "possible values: " + convertSet2String(COLLIMATOR_ORIENTATIONS));
      throw std::runtime_error(format_error(error_info));
    }
  }

  void operator>>(toml::basic_value<toml::type_config> table, std::vector<Collimator> &collimators) {
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
    if (filter.type == "SLAB") {
      return std::unique_ptr<core::Filter>(
        new core::SlabFilter(filter.material,
                             filter.distance,
                             filter.thickness
        )
      );
    }
    throw std::invalid_argument("Unknown filter type: " + filter.type);
  }

  std::unique_ptr<image::ImageHandler> extractExportType(const System &system) {
    if (system.output_type == "TIFF") {
      if (system.pixel_data == "UINT-8") {
        return std::unique_ptr<image::ImageHandler>(new image::TiffUint8Handler());
      }
      if (system.pixel_data == "UINT-16") {
        return std::unique_ptr<image::ImageHandler>(new image::TiffUint16Handler());
      }
      throw std::invalid_argument("Unknown tiff pixel data: " + system.pixel_data);
    }
    if (system.output_type == "DICOM") {
      if (system.pixel_data == "UINT-8") {
        return std::unique_ptr<image::ImageHandler>(new image::DicomUint8Handler(system.use_rescale_slope));
      }
      if (system.pixel_data == "UINT-16") {
        return std::unique_ptr<image::ImageHandler>(new image::DicomUint16Handler(system.use_rescale_slope));
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
