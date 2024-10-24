#include <vector>
#include <iostream>
#include "image.hpp"
#include <algorithm>

Image::Image(int width, int height, const std::vector<int> &input) : _width(width), _height(height), _data(input)
{
}

Image Image::CombineImages(const Image &left, const Image &right, std::function<int(int, int)> func)
{
    if (left._width != right._width || left._height != right._height)
    {
        std::cout << "Images of different sizes";
        return Image(0, 0, std::vector<int>());
    }

    int height = left._height, width = left._width;

    Image result(width, height, std::vector<int>(left._data.size()));

    for (size_t i = 0; i < left._data.size(); i++)
    {
        result._data[i] = func(left._data[i], right._data[i]);
    }

    return result;
}

int Image::GetPixel(int x, int y) const
{
    // TODO:: which is better?
    x = std::max(0, std::min(x, _width - 1));
    y = std::max(0, std::min(y, _height - 1));

    // if (x < 0)
    // {
    //     x *= -1;
    // }
    // else if (x > _width - 1)
    // {
    //     x = _width - 1 - (x - (_width - 1));
    // }

    // if (y < 0)
    // {
    //     y *= -1;
    // }
    // else if (y > _height - 1)
    // {
    //     y = _height - 1 - (y - (_height - 1)); // TODO: Expand
    // }

    return _data.at(y * _width + x); // _data[y * _width + x];
}

void Image::SetPixel(int x, int y, int value)
{
    _data.at(y * _width + x) = value; // _data[y * _width + x] = value;
}

int Image::GetWidth() const { return _width; }

int Image::GetHeight() const { return _height; }

Image Image::Convolve(int size, int *kernel) // TODO const int* ?
{
    Image result(_width, _height, std::vector<int>(_data.size()));

    int halfSize = size / 2;
    for (size_t y = 0; y < _height; y++)
    {
        for (size_t x = 0; x < _width; x++)
        {
            int sum = 0;
            for (size_t kernelY = 0; kernelY < size; kernelY++)
            {
                for (size_t kernelX = 0; kernelX < size; kernelX++)
                {
                    int offsetX = x + kernelX - halfSize;
                    int offsetY = y + kernelY - halfSize;
                    sum += kernel[kernelY * size + kernelX] * GetPixel(offsetX, offsetY);
                }
            }
            result.SetPixel(x, y, sum);
        }
    }

    return result;
}

std::ostream &operator<<(std::ostream &strm, const Image &a)
{
    int height = a.GetHeight();
    int width = a.GetWidth();

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            strm << a.GetPixel(x, y) << " ";
        }
        strm << "\n";
    }

    return strm;
}