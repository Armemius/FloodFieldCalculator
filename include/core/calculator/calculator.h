#pragma once

#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>

#include "core/collimator/collimator.h"
#include "core/detector/detector.h"
#include "core/filter/filter.h"
#include "spectrum/spectrum.h"

namespace pwn::ffc::core {

  /**
   * @brief The Calculator class is an abstract base class for performing
   *        computations involving detectors, filters, collimators, and spectra.
   */
  class Calculator {
    /**
     * @brief Checks the internal state of the calculator for consistency.
     */
    void check() const;

  protected:
    std::unique_ptr<Detector> m_detector = nullptr;    ///< Unique pointer to a Detector object.
    config::Spectrum m_spectrum = {};                  ///< Configuration for the spectrum.
    std::vector<std::unique_ptr<Filter> > m_filters;   ///< Vector of unique pointers to Filter objects.
    std::vector<std::unique_ptr<Collimator> > m_collimators; ///< Vector of unique pointers to Collimator objects.

    /**
     * @brief Processes the data using the current configuration.
     *
     * @return A cv::Mat object containing the processed data.
     */
    virtual cv::Mat process() = 0;

  public:
    /**
     * @brief Virtual destructor for the Calculator class.
     */
    virtual ~Calculator() = default;

    /**
     * @brief Default constructor for the Calculator class.
     */
    Calculator() = default;

    /**
     * @brief Clears the current configuration of the calculator.
     */
    virtual void clear();

    /**
     * @brief Sets the spectrum configuration for the calculator.
     *
     * @param spectrum The spectrum configuration to set.
     * @return A pointer to the current Calculator instance.
     */
    virtual Calculator *setSpectrum(const config::Spectrum &spectrum);

    /**
     * @brief Sets the detector for the calculator.
     *
     * @param detector A unique pointer to the Detector to set.
     * @return A pointer to the current Calculator instance.
     */
    virtual Calculator *setDetector(std::unique_ptr<Detector> detector);

    /**
     * @brief Adds a filter to the calculator.
     *
     * @param filter A unique pointer to the Filter to add.
     * @return A pointer to the current Calculator instance.
     */
    virtual Calculator *addFilter(std::unique_ptr<Filter> filter);

    /**
     * @brief Adds a collimator to the calculator.
     *
     * @param collimator A unique pointer to the Collimator to add.
     * @return A pointer to the current Calculator instance.
     */
    virtual Calculator *addCollimator(std::unique_ptr<Collimator> collimator);

    /**
     * @brief Calculates the field based on the current configuration.
     *
     * @return A cv::Mat object containing the calculated field.
     */
    cv::Mat calculateField();
  };
}
