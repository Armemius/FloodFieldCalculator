#pragma once

#include <set>
#include <unordered_map>

#include "nlohmann/json.hpp"

namespace ffc {
    class Spectrum_handler {
        static Spectrum_handler *_instance;
        std::unordered_map<std::string, std::vector<double>> _coef_map;
        std::vector<double> _attenuation_coefficients;
        bool _use_spectrum_file = false;
        std::set<std::string> _used_materials;
    public:
            static Spectrum_handler *get_instance();

        const std::vector<double> &get_attenuation_coefficients(const std::string &material);

        const std::vector<double> &get_coefficients();

        std::vector<std::string> get_materials();

        void parse_csv_spectres(const std::string &path);

        bool is_in_use() const;

        void add_material(const std::string &material);

    };
}
