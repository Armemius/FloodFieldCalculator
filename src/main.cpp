#include "nlohmann/json.hpp"
#include "opencv_handler.h"
#include <fstream>
#include <iostream>

#include "calculator.h"
#include "config_parser.h"
#include "spectrum_handler.h"
#include "vincentlaucsb/csv.hpp"

int main(const int argc, char *argv[]) {
    disable_logging();

    std::string output_file = "frame";

    if (argc < 2) {
        std::cerr << "No config file provided" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Unable to open config file" << std::endl;
        return EXIT_FAILURE;
    }
    nlohmann::json config;
    file >> config;

    ffc::System_config system_config;

    try {
        system_config = ffc::parse_config(config);
    } catch (const std::invalid_argument &ex) {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (system_config.additional_fields) {
        std::cout << "Additional fields calculation - enabled" << std::endl;
    } else {
        std::cout << "Additional fields calculation - disabled" << std::endl;
    }

    auto used_material = ffc::Spectrum_handler::get_instance()->get_materials();

    const auto &frame = ffc::System::get_instance()->calculate();
    export_frame(frame, "flood_field", system_config.target_width, system_config.target_height,
                 system_config.blur_radius, system_config.output_type, system_config.K, system_config.invert);

    std::cout << "Successfully calculated new flood field" << std::endl;

    for (const auto &it : used_material) {
        const auto &additional_frame = ffc::System::get_instance()->calculate(it);
        export_additional_frame(additional_frame, "flood_field." + it, system_config.target_width, system_config.target_height, system_config.additional_output_type);

        std::cout << "Successfully calculated additional flood field for material '" << it << "'" << std::endl;
    }

    return EXIT_SUCCESS;
}
