#include "spectrum/spectrum.h"

#include <stdexcept>
#include <rapidcsv.h>
#include <spdlog/spdlog.h>

namespace pwn::ffc::config {
  void Spectrum::addMaterial(const Material &material) {
    if (material.channels != this->channels_) {
      throw std::invalid_argument("Material channels count and spectrum channels count does not match");
    }
    this->m_materials.insert({material.name, material});
  }

  std::vector<Material> Spectrum::getMaterials() {
    std::vector<Material> materials;
    for (const auto &[id, material] : this->m_materials) {
      materials.emplace_back(material);
    }
    return materials;
  }

  void Spectrum::readFromCsv(const std::string &csvPath) {
    try {
      const rapidcsv::Document spectrum(csvPath, rapidcsv::LabelParams(0, -1), rapidcsv::SeparatorParams(','));
      const auto &columns = spectrum.GetColumnNames();
      const auto registrationCoefficients = spectrum.GetColumn<double>("Freg");
      this->channels_ = static_cast<int>(registrationCoefficients.size());
      this->m_registration_coefficients = registrationCoefficients;
      for (const auto &it: columns) {
        if (it == "EkV" || it == "Freg") {
          continue;
        }
        const auto coefficients = spectrum.GetColumn<double>(it);
        this->addMaterial({it, static_cast<int>(coefficients.size()), coefficients});
      }
    } catch (const std::exception &err) {
      throw std::runtime_error("Error processing spectrum file: " + std::string(err.what()));
    }
  }

  const std::vector<double> &Spectrum::getRegistrationCoefficients() const {
    return this->m_registration_coefficients;
  }

  const std::vector<double> &Spectrum::getMaterialCoefficients(const std::string &materialId) const {
    if (this->m_materials.find(materialId) == this->m_materials.end()) {
      throw std::invalid_argument("Material '" + materialId + "' is not found");
    }
    return this->m_materials.at(materialId).coefficients;
  }
}
