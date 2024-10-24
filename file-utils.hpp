#pragma once
#include "image.hpp"

namespace FileUtils
{
    Image<int> LoadImage(std::string filename);

    void SaveImage(Image<int> image, std::string filename);
}