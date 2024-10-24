#pragma once
#include <vector>
#include <fstream>
#include <vector>
#include "image.hpp"

class ImageFactory
{
public:
    virtual Image GetImage() = 0;
};

class PredefinedImageFactory : ImageFactory
{
public:
    virtual Image GetImage()
    {
        std::vector<int> v = {10, 20, 30, 40};
        return Image(2, 2, v);
    }
};

class TextFileImageFactory : ImageFactory
{
public:
    TextFileImageFactory(const std::string &filename) : _filename(filename)
    {
    }

    virtual Image GetImage()
    {
        int width, height;
        std::vector<int> data(width * height);

        std::ifstream file(_filename);
        file >> width >> height;

        int n;
        for (size_t i = 0; i < width * height; i++)
        {
            file >> n;
            data.push_back(n);
        }

        return Image(width, height, data);
    }

private:
    std::string _filename;
};