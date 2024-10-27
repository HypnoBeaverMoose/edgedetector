#pragma once
#include <vector>
#include <fstream>
#include <vector>
#include "image.hpp"
#include "file-utils.hpp"

template <typename T>
class ImageFactory
{
public:
    virtual Image<T> GetImage() = 0;
};

class PredefinedImageFactory : ImageFactory<int>
{
public:
    virtual Image<int> GetImage()
    {
        std::vector<int> v = {
            1, 1, 3, 5, 7, 9, 9, 9, 9, 9,
            1, 1, 1, 3, 5, 7, 9, 9, 9, 9,
            1, 1, 1, 1, 3, 5, 7, 9, 9, 9,
            1, 1, 1, 1, 1, 3, 5, 7, 9, 9};

        return Image<int>(10, 4, v);
    }
};

class TgaImageFactory : ImageFactory<float>
{
public:
    TgaImageFactory(const std::string &filename) : _filename(filename)
    {
    }

    virtual Image<float> GetImage()
    {
        return FileUtils::LoadImage(_filename);
    }

private:
    std::string _filename;
};