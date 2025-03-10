# Edge Detector

A hobby implementation of the Canny edge detection algorithm using C++

## Features

- Fast and efficient edge detection using the Canny algorithm
- Support for TGA image format loading and saving

## Requirements

- CMake 3.10 or higher
- C++17 compatible compiler
- Visual Studio Code with CMake Tools extension (recommended)

## Building from Source

### macOS

1. Install dependencies:
```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake
brew install cmake
```

2. Build the project:
```bash
# Clone the repository
git clone https://github.com/yourusername/edgedetector.git
cd edgedetector

# Create build directory
mkdir build && cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Windows

1. Install required tools:
   - Download and install [CMake](https://cmake.org/download/)
   - Install [Visual Studio](https://visualstudio.microsoft.com/) with C++ development tools
   - Add CMake to your system PATH

2. Build the project:
```powershell
# Clone the repository
git clone https://github.com/yourusername/edgedetector.git
cd edgedetector

# Create build directory
mkdir build
cd build

# Configure and build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

### Linux

1. Install dependencies:
```bash
# Update package list
sudo apt-get update

# Install required packages
sudo apt-get install -y cmake build-essential
```

2. Build the project:
```bash
# Clone the repository
git clone https://github.com/yourusername/edgedetector.git
cd edgedetector

# Create build directory
mkdir build && cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Usage

Run the edge detector with:

```bash
# macOS/Linux
./edgedetector input.tga output.tga 0.1

# Windows
edgedetector.exe input.tga output.tga 0.1
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
