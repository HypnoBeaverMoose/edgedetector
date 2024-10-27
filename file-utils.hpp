#pragma once
#include "image.hpp"

namespace FileUtils
{
    /// @brief Loads a TGA image from disk
    /// @param filename path to the image, including extention
    /// @return Image<float> containing the image data
    Image<float> LoadImage(std::string filename);

    /// @brief Saves an image to a TGA file on disk.
    void SaveImage(Image<float> image, std::string filename);
}