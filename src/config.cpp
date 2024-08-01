#include "config.h"

#include <exception>

namespace pwn::ffc::config {
  static std::string convertSet2String(const std::unordered_set<std::string> &set) {
    std::string res = "[";
    auto jt = FILTER_TYPES.begin();
    for (size_t it = 0; it < FILTER_TYPES.size(); ++it, ++jt) {
      res += *jt;
      if (it < FILTER_TYPES.size() - 1) {
        res += ", ";
      }
    }
    res += "]";
    return res;
  }

  void operator>>(toml::basic_value<toml::type_config> table, Resolution &resolution) {
    resolution.height = toml::find<int>(table, "height");
    resolution.width = toml::find<int>(table, "width");
  }

  void operator>>(toml::basic_value<toml::type_config> table, System &system) {
    system.outputFilename = toml::find_or<std::string>(table, "output-filename", "flood_field");
    system.outputType = toml::find<std::string>(table, "output-type");
    system.pixelData = toml::find<std::string>(table, "pixel-data");
    system.blurRadius = toml::find_or<int>(table, "blur-radius", 0);
    system.scalingCoefficient = toml::find<double>(table, "scaling-coefficient");
    system.spectrumTable = toml::find<std::string>(table, "spectrum-table");
    system.invert = toml::find_or<bool>(table, "invert", false);
    find(table, "target-resolution") >> system.targetResolution;
  }

  void operator>>(toml::basic_value<toml::type_config> table, Detector &detector) {
    detector.type = toml::find<std::string>(table, "type");
    detector.distance = toml::find<double>(table, "distance");
    detector.radius = toml::find<double>(table, "radius");
    find(table, "resolution") >> detector.resolution;
    find(table, "size") >> detector.size;
  }

  void operator>>(toml::basic_value<toml::type_config> table, Filter &filter) {
    filter.type = toml::find<std::string>(table, "type");
    if (FILTER_TYPES.find(filter.type) == FILTER_TYPES.end()) {
      const auto errorInfo = make_error_info("Unknown filter type: " + filter.type,
                                             find(table, "type"),
                                             "at this row",
                                             "possible values: " + convertSet2String(FILTER_TYPES));
      throw std::runtime_error(format_error(errorInfo));
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
      const auto errorInfo = make_error_info("Unknown collimator type: " + collimator.type,
                                             find(table, "type"),
                                             "at this row",
                                             "possible values: " + convertSet2String(COLLIMATOR_TYPES));
      throw std::runtime_error(format_error(errorInfo));
    }
    if (COLLIMATOR_ORIENTATIONS.find(collimator.orientation) == COLLIMATOR_ORIENTATIONS.end()) {
      const auto errorInfo = make_error_info("Unknown collimator orientation type: " + collimator.type,
                                             find(table, "orientation"),
                                             "at this row",
                                             "possible values: " + convertSet2String(COLLIMATOR_ORIENTATIONS));
      throw std::runtime_error(format_error(errorInfo));
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
}
