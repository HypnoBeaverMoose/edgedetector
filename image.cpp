#include <vector>
#include <iostream>
#include "image.hpp"

Image::Image(int width, int height, const std::vector<int> &input) : _width(width), _height(height), _data(input)
{
}

int Image::GetPixel(int x, int y) const
{
    if (x < 0)
    {
        x *= -1;
    }
    else if (x > _width - 1)
    {
        x = _width - (x - (_width - 1)); // TODO: Expand
    }

    if (y < 0)
    {
        y *= -1;
    }
    else if (y > _height - 1)
    {
        y = _height - (y - (_height - 1)); // TODO: Expand
    }

    return _data.at(y * (_width - 1) + x); // _data[y * _width + x];
}

void Image::SetPixel(int x, int y, int value)
{
    _data.at(y * (_width - 1) + x) = value; // _data[y * _width + x] = value;
} 

int Image::GetWidth() const { return _width; }

int Image::GetHeight() const { return _height; }

void Image::Convolve(int size, int *kernel) // TODO const int* ?
{
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
            SetPixel(x, y, sum);
        }
    }
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