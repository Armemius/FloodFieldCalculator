#include <iostream>

#include <argparse/argparse.hpp>
#include <spdlog/spdlog.h>

#include "config.h"
#include "core/calculator/attenuation_calculator.h"
#include "core/calculator/material_distance_calculator.h"
#include "spectrum/spectrum.h"

/**
 * Configures argparser instance for later use
 *
 * Writes error message to stderr if invalid configuration file was provided
 * and help message/version if corresponding flags are present
 *
 * @param program argparse::ArgumentParser instance to be initialized
 * @param argc number of arguments passed to the program
 * @param argv array of argruments passed to the program
 * @return true if configuration was succesfull, false otherwise
 */
bool configureArgparser(argparse::ArgumentParser &program, const int argc, char *argv[]) noexcept {
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

int main(const int argc, char *argv[]) {
  try {
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

    const auto &configPath = program.get<std::string>("--config");
    pwn::ffc::config::Config config;
    spdlog::debug("Parsing given config file ({})", configPath);
    try {
      config = pwn::ffc::config::parseConfig(configPath);
    } catch (const std::runtime_error &ex) {
      spdlog::error(ex.what());
      return EXIT_FAILURE;
    }
    spdlog::debug("{} filter(s) and {} collimator(s) attached",
                  config.filters.size(),
                  config.collimators.size());

    pwn::ffc::config::Spectrum spectrum;
    try {
      spectrum.readFromCsv(config.system.spectrum_table);
    } catch (const std::runtime_error &ex) {
      spdlog::error(ex.what());
      return EXIT_FAILURE;
    }

    pwn::ffc::core::AttenuationCalculator attenuation_calculator(config.system.logarithmize,
                                                                 config.system.scaling_coefficient);
    attenuation_calculator.setSpectrum(spectrum);
    attenuation_calculator.setDetector(extractDetector(config.detector));
    for (const auto &it: config.filters) {
      attenuation_calculator.addFilter(extractFilter(it));
    }

    const auto main_field = attenuation_calculator.calculateField();
    const auto image_handler = extractExportType(config.system);
    image_handler->outputImage(main_field, config.system.output_filename);

    if (config.system.additional_fields) {
      std::unordered_set<std::string> materials;
      for (const auto &[type, thickness, distance, material]: config.filters) {
        materials.insert(material);
      }
      spdlog::info("Calculating additional fields for {} material(s)", materials.size());
      std::vector<std::pair<std::string, std::unique_ptr<pwn::ffc::core::Calculator> > > additional_fields_calculators;
      for (const auto &material: materials) {
        additional_fields_calculators.emplace_back(
          material,
          std::unique_ptr<pwn::ffc::core::Calculator>(new pwn::ffc::core::MaterialDistanceCalculator(material))
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
      }
    }

    return EXIT_SUCCESS;
  } catch (const std::exception &ex) {
    spdlog::error("Unhandled error! {}", ex.what());
    return EXIT_FAILURE;
  }
}
