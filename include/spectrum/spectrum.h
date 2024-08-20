#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "material.h"

namespace pwn::ffc::config {
  /// Struct containing information about spectral setup
  class Spectrum {
    /// Number of spectral channels
    int channels_ = 1;
    /// Registration coefficients for spectral channels
    std::vector<double> m_registration_coefficients = {1.0};
    /// Map that matches material ID and corresponding Material
    std::unordered_map<std::string, Material> m_materials = {};

  public:
    /**
     * Adds new material to the list and registers it in current spectrum
     *
     * @param material material to be added
     * @throws invalid_argument if spectrum channel count and material channel count does not match
     */
    void addMaterial(const Material &material);

    /**
     * Returns list of materials attached to spectrum
     *
     * @return list of materials attached to spectrum
     */
    std::vector<Material> getMaterials();

    /**
     * Reads and configures spectrum instance with the data provided in a CSV file
     *
     * CSV separator should be comma (',')
     *
     * @param csvPath path to CSV file containing spectrum information
     */
    void readFromCsv(const std::string &csvPath);

    /**
     * Returns spectrum registration coefficients (F_reg values) for each channel
     *
     * @return spectrum registration coefficients
     */
    [[nodiscard]] const std::vector<double> &getRegistrationCoefficients() const;

   void setRegistrationCoefficients(std::vector<double> coefficients);

    /**
     * Returns material attenuation coefficients for each channel
     *
     * @param materialId material id for coefficients to look for
     * @return material attenuation coefficients
     * @throws invalid_argument if specified material is not found
     */
    [[discard]] const std::vector<double> &getMaterialCoefficients(const std::string &materialId) const;
  };
}
