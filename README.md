# FloodFieldCalculator

FloodFieldCalculator is a command-line application designed to calculate flood fields using various detectors, filters, and collimators. This application reads configuration settings from a TOML file, processes the input data, and outputs the calculated flood fields.

## Features

- Configurable via a TOML file
- Supports silent and verbose modes
- Reads spectrum data from CSV files
- Supports multiple filters and collimators
- Outputs results in various image formats
- Optionally calculates additional fields for different materials

## Requirements

- C++17 or later
- [argparse](https://github.com/p-ranav/argparse) for argument parsing
- [spdlog](https://github.com/gabime/spdlog) for logging
- [OpenCV](https://opencv.org/) for image processing
- [TOML11](https://github.com/ToruNiina/toml11) for parsing TOML configuration files
- [DCMTK](https://github.com/DCMTK/dcmtk) for working with DICOM data
- [Conan](https://conan.io/) for working with DICOM data

## Usage

To run the FloodFieldCalculator, execute the following command:

```bash
FloodFieldCalculator.exe --config path/to/config.toml [--silent] [--verbose]
```

### Arguments

- `-c`, `--config`: Path to the TOML configuration file. This argument is required.
- `-s`, `--silent`: Disables program output. This argument is optional.
- `-v`, `--verbose`: Enables additional output. This argument is optional.

## Configuration

The configuration file should be written in TOML format and should specify the necessary settings for the spectrum, detector, filters, collimators, and other system parameters. Below is an example configuration file:

```toml
[system]
output-filename = "flood_field"
output-type = "TIFF"
pixel-data = "UINT-8"
additional-fields = true
blur-radius = 255
spectrum-table = "C:/PAWLIN/spectrum.csv"
invert = false
logarithmize = true
scaling-coefficient = 10000.0
use-rescale-slope = true

[system.target-resolution]
width = 1024
height = 1024

[detector]
type = "CURVED"
distance = 1040.0
radius = 1040.0

[detector.resolution]
width = 256
height = 256

[detector.size]
width = 1024
height = 1024

[[filter]]
type = "SLAB"
distance = 20.0
thickness = 0.03
material = "Cu"

[[collimator]]
orientation = "VERTICAL"
type = "SYMMETRICAL"
distance = 1000.0
shift = 800.0



```

## Example

To run the calculator with a specific configuration file:

```bash
FloodFieldCalculator.exe --config config.toml --verbose
```

## Logging

FloodFieldCalculator uses `spdlog` for logging. By default, the application starts with logging disabled. Use the `--verbose` flag to enable debug messages or the `--silent` flag to disable all messages.

## Development

### Building

Ensure you have CMake installed and configured. Create a build directory and run CMake:

```bash
mkdir build
cd build
conan install ..
cmake ..
make
```

## License

TODO
