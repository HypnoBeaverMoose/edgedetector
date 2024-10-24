#pragma once
#include <vector>
#include <fstream>
#include <vector>
#include "image.hpp"

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
        std::vector<int> v = {  1, 1, 1, 1, 1, 1,
                                1, 5, 5, 5, 5, 1,
                                1, 5, 5, 5, 5, 1,
                                1, 5, 5, 5, 5, 1,
                                1, 1, 1, 1, 1, 1,
                             };
        return Image<int>(6, 5, v);

        // std::vector<int> v = {  5, 1, 1, 1, 1,
        //                         10, 5, 1, 1,1,
        //                         10, 10, 5, 1,1,
        //                         10, 10, 10, 5,1,
        //                         10, 10, 10, 10,5,
        //                      };
        // return Image(5, 5, v);

        // std::vector<int> v = {  
        //                         1, 1, 3, 5, 7, 9, 9, 9, 9, 9,
        //                         1, 1, 1, 3, 5, 7, 9, 9, 9, 9,
        //                         1, 1, 1, 1, 3, 5, 7, 9, 9, 9,
        //                         1, 1, 1, 1, 1, 3, 5, 7, 9, 9
        //                      };
        // return Image(10, 4, v);
    }
};

class TextFileImageFactory : ImageFactory<int>
{
public:
    TextFileImageFactory(const std::string &filename) : _filename(filename)
    {
    }

    virtual Image<int> GetImage()
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

        return Image<int>(width, height, data);
    }

private:
    std::string _filename;
};