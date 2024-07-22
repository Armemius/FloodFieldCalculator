#include "config_parser.h"

#include <iostream>

#include "calculator.h"
#include "spectrum_handler.h"
#include "collimator/horizontal_gap_collimator.h"
#include "collimator/vertical_gap_collimator.h"
#include "detector/curved_detector.h"
#include "detector/flat_detector.h"
#include "filter/curved_filter.h"
#include "filter/cylindrical_filter.h"
#include "filter/gauss_filter.h"
#include "filter/slab_filter.h"
#include "collimator/asymmetrical_vertical_gap_collimator_left.h"
#include "collimator/asymmetrical_vertical_gap_collimator_right.h"

static bool check_value(const nlohmann::basic_json<> &json, const std::string &field) {
    return json.find(field) != json.end();
}

template<typename T>
static T get_config_value(const nlohmann::basic_json<> &json, const std::string &field) {
    if (!check_value(json, field)) {
        throw std::invalid_argument("Value for '" + field + "' wasn't provided");
    }
    return json[field];
}

static void parse_detector(const nlohmann::json &config) {
    const auto detecor_config = get_config_value<nlohmann::basic_json<> >(
            config, "detector");
    const auto distance = get_config_value<double>(detecor_config, "distance");
    const auto type = get_config_value<std::string>(detecor_config, "type");
    const auto &resolution = get_config_value<nlohmann::basic_json<> >(detecor_config, "resolution");
    const int resolution_width = get_config_value<int>(resolution, "width");
    const int resolution_height = get_config_value<int>(resolution, "height");
    const auto &size = get_config_value<nlohmann::basic_json<> >(detecor_config, "size");
    const auto size_width = get_config_value<double>(size, "width");
    const auto size_height = get_config_value<double>(size, "height");
    if (type == "FLAT") {
        ffc::System::get_instance()->set_detector(
                new ffc::Flat_detector(distance,
                                       {resolution_width, resolution_height},
                                       {size_width, size_height},
                                       {0, 0}));
        return;
    }
    if (type == "CURVED") {
        const auto radius = get_config_value<double>(detecor_config, "radius");
        ffc::System::get_instance()->set_detector(
                new ffc::Curved_detector(distance,
                                         {resolution_width, resolution_height},
                                         {size_width, size_height},
                                         {0, 0}, radius));
        return;
    }
    throw std::invalid_argument("Unknown detector type: " + type);
}

static void parse_signal(const nlohmann::json &config) {
    if (ffc::Spectrum_handler::get_instance()->is_in_use()) {
        if (check_value(config, "signal")) {
            std::cout << "Using coefficients from spectrum config, ignoring json config values" << std::endl;
        }
        ffc::System::get_instance()->set_signal(
                new ffc::Signal(ffc::Spectrum_handler::get_instance()->get_coefficients()));
        return;
    }
    const auto &coefficients = get_config_value<std::vector<double> >(config, "signal");
    ffc::System::get_instance()->set_signal(new ffc::Signal(coefficients));
}

static void parse_filters(const nlohmann::json &config) {
    const auto &filters = get_config_value<nlohmann::json>(config, "filters");
    for (const auto &filter: filters) {
        const std::string &type = get_config_value<std::string>(filter, "type");
        const std::vector<double> &coefficients = ffc::Spectrum_handler::get_instance()->is_in_use()
                                                  ? ffc::Spectrum_handler::get_instance()->
                        get_attenuation_coefficients(get_config_value<std::string>(
                        filter, "attenuation-coefficients"))
                                                  : get_config_value<std::vector<double> >(
                        filter, "attenuation-coefficients");
        if (ffc::Spectrum_handler::get_instance()->is_in_use()) {
            ffc::Spectrum_handler::get_instance()->add_material(get_config_value<std::string>(
                    filter, "attenuation-coefficients"));
        }
        const auto distance = get_config_value<double>(filter, "distance");
        ffc::Filter *new_filter;
        if (type == "FLAT") {
            const auto thickness = get_config_value<double>(filter, "thickness");
            new_filter = new ffc::Slab_filter(thickness, distance, coefficients);
            if (ffc::Spectrum_handler::get_instance()->is_in_use()) {
                new_filter->set_material(get_config_value<std::string>(
                        filter, "attenuation-coefficients"));
            }
            ffc::System::get_instance()->add_filter(new_filter);
            continue;
        }
        if (type == "PARABOLIC") {
            const auto min_thickness = get_config_value<double>(filter, "min-thickness");
            const auto max_thickness = get_config_value<double>(filter, "max-thickness");
            const auto radius = get_config_value<double>(filter, "radius");
            new_filter = new ffc::Curved_filter(min_thickness, max_thickness, distance, radius, coefficients);
            if (ffc::Spectrum_handler::get_instance()->is_in_use()) {
                new_filter->set_material(get_config_value<std::string>(
                        filter, "attenuation-coefficients"));
            }
            ffc::System::get_instance()->add_filter(new_filter);
            continue;
        }
        if (type == "CYLINDRICAL") {
            const auto thickness = get_config_value<double>(filter, "thickness");
            const auto radius = get_config_value<double>(filter, "radius");
            new_filter = new ffc::Cylindrical_filter(thickness, distance, radius, coefficients);
            if (ffc::Spectrum_handler::get_instance()->is_in_use()) {
                new_filter->set_material(get_config_value<std::string>(
                        filter, "attenuation-coefficients"));
            }
            ffc::System::get_instance()->add_filter(new_filter);
            continue;
        }
        if (type == "GAUSS") {
            const auto thickness = get_config_value<double>(filter, "thickness");
            const auto depth = get_config_value<double>(filter, "depth");
            const auto sigma = get_config_value<double>(filter, "sigma");
            new_filter = new ffc::Gauss_filter(thickness, distance, sigma, depth, coefficients);
            if (ffc::Spectrum_handler::get_instance()->is_in_use()) {
                new_filter->set_material(get_config_value<std::string>(
                        filter, "attenuation-coefficients"));
            }
            ffc::System::get_instance()->add_filter(new_filter);
            continue;
        }
        throw std::invalid_argument("Unknown filter type: " + type);
    }
}

static void parse_collimators(const nlohmann::json &config) {
    const auto &collimators = get_config_value<nlohmann::json>(config, "collimators");
    for (const auto &collimator: collimators) {
        const std::string &type = get_config_value<std::string>(collimator, "type");

        const auto offset = get_config_value<double>(collimator, "distance");
        const auto gap = get_config_value<double>(collimator, "gap");

        if (type == "VERTICAL") {
            ffc::System::get_instance()->add_collimator(new ffc::Vertical_gap_collimator(gap, offset));
            continue;
        }
        if (type == "VERTICAL-ASYMMETRIC-LEFT") {
            ffc::System::get_instance()->add_collimator(new ffc::Vertical_left_gap_collimator(gap, offset));
            continue;
        }
        if (type == "VERTICAL-ASYMMETRIC-RIGHT") {
            ffc::System::get_instance()->add_collimator(new ffc::Vertical_right_gap_collimator(gap, offset));
            continue;
        }
        if (type == "HORIZONTAL") {
            ffc::System::get_instance()->add_collimator(new ffc::Horizontal_gap_collimator(gap, offset));
            continue;
        }

        throw std::invalid_argument("Unknown collimator type: " + type);
    }
}

static ffc::System_config parse_system(const nlohmann::json &config) {
    const auto &system = get_config_value<nlohmann::basic_json<> >(config, "system");
    const auto &target_resolution = get_config_value<nlohmann::basic_json<> >(system, "target-resolution");

    const int width = get_config_value<int>(target_resolution, "width");
    const int height = get_config_value<int>(target_resolution, "height");

    const std::string &type = get_config_value<std::string>(system, "output-type");
    std::string additional_type;
    if (check_value(system, "additional-fields-output-type")) {
        additional_type = get_config_value<std::string>(system, "additional-fields-output-type");
    } else {
        additional_type = type;
    }
    const int blur_radius = get_config_value<int>(system, "blur-radius");

    double K = 1.0;
    std::string flood_field_path = "null";

    if (check_value(system, "K")) {
        K = get_config_value<double>(system, "K");
    } else if (check_value(system, "flood_field0")) {
        flood_field_path = get_config_value<std::string>(system, "flood_field0");
    } else {
        std::cout << "K parameter and path to flood_field0 are not specified, defaulting to K parameter with value 1.0"
                  << std::endl;
    }

    if (check_value(system, "csv-path")) {
        const std::string &csv_path = get_config_value<std::string>(system, "csv-path");
        ffc::Spectrum_handler::get_instance()->parse_csv_spectres(csv_path);
    }

    bool additional_fields = false;

    if (check_value(system, "additional-fields")) {
        additional_fields = get_config_value<bool>(system, "additional-fields");
    }

    bool inversion = false;

    if (check_value(system, "invert")) {
        inversion = get_config_value<bool>(system, "invert");
    }

    return {
            blur_radius,
            width,
            height,
            type,
            additional_type,
            K,
            flood_field_path,
            additional_fields,
            inversion
    };
}

ffc::System_config ffc::parse_config(const nlohmann::json &config) {
    System_config system_config;
    try {
        system_config = parse_system(config);
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error while parsing system config: " << ex.what() << std::endl;
        throw std::invalid_argument("Invalid config provided");
    }
    try {
        parse_signal(config);
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error while parsing signal config: " << ex.what() << std::endl;
        throw std::invalid_argument("Invalid config provided");
    }
    try {
        parse_detector(config);
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error while parsing detector config: " << ex.what() << std::endl;
        throw std::invalid_argument("Invalid config provided");
    }
    try {
        parse_filters(config);
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error while parsing filters config: " << ex.what() << std::endl;
        throw std::invalid_argument("Invalid config provided");
    }
    try {
        parse_collimators(config);
    } catch (const std::invalid_argument &ex) {
        std::cerr << "Error while parsing collimators config: " << ex.what() << std::endl;
        throw std::invalid_argument("Invalid config provided");
    }
    return system_config;
}
