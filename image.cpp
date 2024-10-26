#include <vector>
#include <iostream>
#include "image.hpp"
#include <algorithm>

template <typename T>
void Image<T>::CombineImages(const Image<T> &left, const Image<T> &right, Image<T>& result, std::function<T(T, T)> func)
{
    if (left._width != right._width || left._height != right._height || left._width != result._width || left._height != result._height)
    {
        std::cerr << "Error: Trying to combine images of different dimensions.";
        exit(1);
    }

    int height = left._height, width = left._width;

    for (size_t i = 0; i < left._data.size(); i++)
    {
        result._data[i] = func(left._data[i], right._data[i]);
    }
}

template <typename T>
T Image<T>::GetPixel(int x, int y, OverflowStrategy overflow, T def) const
{
    switch (overflow)
    {
    case DEFAULT:
        if (x < 0 || x >= _width || y < 0 || y >= _height)
        {
            return def;
        }
        break;
    case CLAMP:
        x = std::max(0, std::min(x, _width - 1));
        y = std::max(0, std::min(y, _height - 1));
        break;
    case MIRROR:
        if (x < 0)
        {
            x *= -1;
        }
        else if (x > _width - 1)
        {
            x = 2 * _width - 2 - x;
        }
        if (y < 0)
        {
            y *= -1;
        }
        else if (y > _height - 1)
        {
            y = 2 * _height - 2 - y;
        }
        break;
    default:
        break;
    }

    return _data.at(y * _width + x);
}

template <typename T>
void Image<T>::SetPixel(int x, int y, T value)
{
    _data.at(y * _width + x) = value;
}

template <typename T>
int Image<T>::GetWidth() const
{
    return _width;
}

template <typename T>
int Image<T>::GetHeight() const
{
    return _height;
}

template <typename T>
Image<T> Image<T>::GetConvolved(const std::vector<T> &kernel) const
{
    Image result(_width, _height, std::vector<T>(_data.size()));

    int size = kernel.size();
    int halfSize = size / 2;
    for (size_t y = 0; y < _height; y++)
    {
        for (size_t x = 0; x < _width; x++)
        {
            T sum = 0;
            for (size_t kernelY = 0; kernelY < size; kernelY++)
            {
                for (size_t kernelX = 0; kernelX < size; kernelX++)
                {
                    int offsetX = x + kernelX - halfSize;
                    int offsetY = y + kernelY - halfSize;
                    sum += kernel[kernelY * size + kernelX] * GetPixel(offsetX, offsetY, CLAMP);
                }
            }
            result.SetPixel(x, y, sum);
        }
    }

    return result;
}

template <typename T>
Image<T> Image<T>::GetConvolvedSeparable(const std::vector<T> &kernelX, const std::vector<T> &kernelY) const
{
    Image intermediate1(_width, _height);
    Image intermediate2(_width, _height);

    int size = kernelX.size();
    int halfSize = size / 2;
    for (size_t y = 0; y < _height; y++)
    {
        for (size_t x = 0; x < _width; x++)
        {
            T sum = 0;
            for (size_t kernelOffset = 0; kernelOffset < size; kernelOffset++)
            {
                int offsetX = x + kernelOffset - halfSize;
                sum += kernelX[kernelOffset] * GetPixel(offsetX, y, CLAMP);
            }
            intermediate1.SetPixel(x, y, sum);
        }
    }

    for (size_t y = 0; y < _height; y++)
    {
        for (size_t x = 0; x < _width; x++)
        {
            T sum = 0;
            for (size_t kernelOffset = 0; kernelOffset < size; kernelOffset++)
            {
                int offsetY = y + kernelOffset - halfSize;
                sum += kernelY[kernelOffset] * intermediate1.GetPixel(x, offsetY, CLAMP);
            }
            intermediate2.SetPixel(x, y, sum);
        }
    }
    return intermediate2;
}

template <typename T>
void Image<T>::Convolve(const std::vector<T> &kernelX, const std::vector<T> &kernelY)
{
    std::vector<T> buffer(std::max(_width, _height));

    int size = kernelX.size();
    int halfSize = size / 2;

    for (size_t y = 0; y < _height; y++)
    {
        for (size_t x = 0; x < _width; x++)
        {
            T sum = 0;
            for (int kernelOffset = -halfSize; kernelOffset <= halfSize; kernelOffset++)
            {
                int offsetX = x + kernelOffset;
                offsetX = std::max(0, std::min(offsetX, _width - 1));
                sum += kernelX[kernelOffset + halfSize] * _data[y * _width + offsetX];
            }
            buffer[x] = sum;
        }

        for (size_t i = 0; i < _width; i++)
        {
            _data[y * _width + i] = buffer[i];
        }
    }

    for (size_t x = 0; x < _width; x++)
    {
        for (size_t y = 0; y < _height; y++)
        {
            T sum = 0;
            for (int kernelOffset = -halfSize; kernelOffset <= halfSize; kernelOffset++)
            {
                int offsetY = y + kernelOffset;
                offsetY = std::max(0, std::min(offsetY, _height - 1));
                sum += kernelY[kernelOffset + halfSize] * _data[offsetY * _width + x];
            }
            buffer[y] = sum;
        }

        for (size_t i = 0; i < _height; i++)
        {
            _data[i * _width + x] = buffer[i];
        }
    }
}

template <typename T>
void Image<T>::Normalize(T normalizer)
{
    T max = *std::max_element(_data.begin(), _data.end());
    for (size_t i = 0; i < _data.size(); i++)
    {
        _data[i] = NormalizeElement(_data[i], max) * normalizer;
    }
}

template <typename T>
void Image<T>::ApplyDoubleThreshold(T low, T high, T max)
{
    for (auto &&pixel : _data)
    {
        pixel = pixel < low ? 0 : pixel > high ? max
                                               : pixel;
    }
}

template <typename T>
T Image<T>::NormalizeElement(T element, T normalizer)
{
    return element / normalizer;
}

template <>
int Image<int>::NormalizeElement(int element, int normalizer)
{
    return std::round(element / (float)normalizer);
}

template <typename T>
std::vector<std::tuple<int, int>> Image<T>::FindNonZeroPixels() const
{
    std::vector<std::tuple<int, int>> result;
    for (size_t i = 0; i < _data.size(); i++)
    {
        if (_data[i] - 0 > std::numeric_limits<T>::epsilon())
        {
            result.push_back({i / _width, i % _width});
        }
    }

    return result;
}

template class Image<int>;
template class Image<float>;