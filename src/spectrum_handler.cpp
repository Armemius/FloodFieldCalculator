#include "spectrum_handler.h"

#include "vincentlaucsb/csv.hpp"

ffc::Spectrum_handler *ffc::Spectrum_handler::_instance = nullptr;

ffc::Spectrum_handler *ffc::Spectrum_handler::get_instance() {
    if (_instance == nullptr) {
        _instance = new Spectrum_handler();
    }
    return _instance;
}

const std::vector<double> &ffc::Spectrum_handler::get_attenuation_coefficients(const std::string &material) {
    if (this->_coef_map.find(material) == this->_coef_map.end()) {
        throw std::invalid_argument("Material '" + material + "' is not present in spectrum config");
    }
    return this->_coef_map[material];
}

const std::vector<double> &ffc::Spectrum_handler::get_coefficients() {
    return this->_attenuation_coefficients;
}

std::vector<std::string> ffc::Spectrum_handler::get_materials() {
    std::vector<std::string> result(_used_materials.size());
    std::copy(_used_materials.begin(), _used_materials.end(), result.begin());
    return result;
}

void ffc::Spectrum_handler::parse_csv_spectres(const std::string &path) {
    csv::CSVReader reader(path);
    const static std::string EKV = "EkV";
    const static std::string F_REG = "Freg";
    for (const auto &it : reader.get_col_names()) {
        if (it == F_REG || it == EKV) {
            continue;
        }
        this->_coef_map[it] = std::vector<double>();
    }
    for (csv::CSVRow& row: reader) {
        for (size_t it = 0; it < row.size(); ++it) {
            const std::string column = reader.get_col_names()[it];
            if (column == EKV) {
                continue;
            }
            if (column == F_REG) {
                this->_attenuation_coefficients.push_back(row[it].get<double>());
            } else {
                this->_coef_map[column].push_back(row[it].get<double>());
            }

        }
    }
    this->_use_spectrum_file = true;
}

bool ffc::Spectrum_handler::is_in_use() const {
    return this->_use_spectrum_file;
}

void ffc::Spectrum_handler::add_material(const std::string &material) {
    this->_used_materials.insert(material);
}
