#include "flood_field_calculator.h"

#include <spdlog/spdlog.h>
#include <opencv2/core/utils/logger.hpp>

#include "config.h"
#include "core/calculator/calculator.h"
#include "core/calculator/attenuation_calculator.h"
#include "core/calculator/material_distance_calculator.h"
#include "image/image_handler.h"
#include "spectrum/spectrum.h"
#include "io/dicom.h"

namespace pwn::ffc::core {
  bool FloodFieldCalculator::configureArgparser(argparse::ArgumentParser &program, const int argc,
                                                char *argv[]) noexcept {
    {
      program.add_argument<std::string>("-c", "--config")
          .help("path to TOML configuration file")
          .metavar("path/to/config.toml")
          .required();

      program.add_argument<std::string>("-s", "--silent")
          .help("disables program output")
          .flag();

      program.add_argument<std::string>("-v", "--verbose")
          .help("enables additional output")
          .flag();

      try {
        program.parse_args(argc, argv);
      } catch (const std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << program;
        return false;
      }
      return true;
    }
  }

  int FloodFieldCalculator::run(int argc, char *argv[]) {
    setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

    argparse::ArgumentParser program("FloodFieldCalculator.exe");
    if (!configureArgparser(program, argc, argv)) {
      return EXIT_FAILURE;
    }

    if (program.get<bool>("--verbose")) {
      spdlog::set_level(spdlog::level::debug);
    }

    if (program.get<bool>("--silent")) {
      spdlog::set_level(spdlog::level::off);
    }

    spdlog::info("Starting FloodFieldCalculator");
    spdlog::debug("Debug messages output enabled");

    const auto &config_path = program.get<std::string>("--config");
    config::Config config;
    spdlog::debug("Parsing given config file ({})", config_path);
    try {
      config = config::parseConfig(config_path);
    } catch (const std::runtime_error &ex) {
      spdlog::error(ex.what());
      return EXIT_FAILURE;
    }

    return run(config);
  }

  int FloodFieldCalculator::run(ffc::config::Config config) {
    config::Spectrum spectrum;
    try {
      spectrum.readFromCsv(config.system.spectrum_table);
    } catch (const std::runtime_error &ex) {
      spdlog::error(ex.what());
      return EXIT_FAILURE;
    }

    spdlog::info("Calculating flood field");
    spdlog::debug("{} filter(s) and {} collimator(s) attached",
                  config.filters.size(),
                  config.collimators.size());
    spdlog::debug("Use logarithmization: {}", config.system.logarithmize);
    spdlog::debug("Use invertion: {}", config.system.invert);
    spdlog::debug("Use rescale slope: {}", config.system.use_rescale_slope);
    spdlog::debug("Output image type: {}", config.system.output_type);
    spdlog::debug("Output image pixel data type: {}", config.system.pixel_data);

    AttenuationCalculator *attenuation_calculator;
    if (config.system.scaling_coefficient != SCALING_COEFFICIENT_UNSET || !config.system.logarithmize) {
      attenuation_calculator = new AttenuationCalculator(config.system.logarithmize,
                                                         cv::Mat(config.detector.resolution.height,
                                                                 config.detector.resolution.width, CV_64FC1,
                                                                 config.system.scaling_coefficient));
    } else {
      const cv::Mat map = io::readDicom(config.system.scaling_coefficient_path).mat;
      attenuation_calculator = new AttenuationCalculator(config.system.logarithmize, map);
    }

    attenuation_calculator->setSpectrum(spectrum);
    attenuation_calculator->setDetector(extractDetector(config.detector));
    try {
      for (const auto &it: config.filters) {
        attenuation_calculator->addFilter(extractFilter(it));
      }
    } catch (const std::runtime_error &ex) {
      spdlog::error("Error while processing filters: {}", ex.what());
    }
    try {
      for (const auto &it: config.collimators) {
        attenuation_calculator->addCollimator(extractCollimator(it));
      }
    } catch (const std::runtime_error &ex) {
      spdlog::error("Error while processing collimators: {}", ex.what());
    }


    auto main_field = attenuation_calculator->calculateField();
    const auto image_handler = extractExportType(config.system);
    spdlog::info("Saving {}...", config.system.output_filename);
    image_handler->outputImage(main_field, config.system.output_filename);

    if (config.system.additional_fields) {
      std::unordered_set<std::string> materials;
      for (const auto &filter: config.filters) {
        materials.insert(filter.material);
      }
      spdlog::info("Calculating additional fields for {} material(s)", materials.size());
      std::vector<std::pair<std::string, std::unique_ptr<Calculator> > > additional_fields_calculators;
      for (const auto &material: materials) {
        additional_fields_calculators.emplace_back(
          material,
          std::unique_ptr<Calculator>(new MaterialDistanceCalculator(material))
        );
      }
      for (auto &[material, calculator]: additional_fields_calculators) {
        spdlog::info("Calculating additional field for {}...", material);
        calculator->setSpectrum(spectrum);
        calculator->setDetector(extractDetector(config.detector));
        for (const auto &filter: config.filters) {
          calculator->addFilter(extractFilter(filter));
        }
        image_handler->outputImage(calculator->calculateField(), config.system.output_filename + "." + material);
        spdlog::info("Saving {}...", config.system.output_filename + "." + material);
      }
    }

    delete attenuation_calculator;
    return EXIT_SUCCESS;
  }
}
