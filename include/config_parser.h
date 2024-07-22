#pragma once

#include "nlohmann/json.hpp"

namespace ffc {

    struct System_config {
        int blur_radius;
        int target_width;
        int target_height;
        std::string output_type;
        std::string additional_output_type;
        double K;
        std::string flood_field_path;
        bool additional_fields;
        bool invert;
    };

    System_config parse_config(const nlohmann::json &config);
}
