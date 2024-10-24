#pragma once
#include "image.hpp"

namespace FileUtils
{
    Image<float> LoadImage(std::string filename);

    void SaveImage(Image<float> image, std::string filename);
}