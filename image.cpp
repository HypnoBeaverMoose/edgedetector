#include <vector>
#include <iostream>
#include "image.hpp"
#include <algorithm>

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
void Image<T>::Convolve(const std::vector<T> &kernelX, const std::vector<T> &kernelY)
{
    std::vector<T> buffer(std::max(_width, _height));

    int size = kernelX.size();
    int halfSize = size / 2;

    for (size_t y = 0; y < _height; y++)
    {
        // Handle general case.
        for (size_t x = halfSize; x < _width - halfSize; x++)
        {
            T sum = 0;
            for (int kernelOffset = -halfSize; kernelOffset <= halfSize; kernelOffset++)
            {
                int offsetX = x + kernelOffset;
                sum += kernelX[kernelOffset + halfSize] * _data[y * _width + offsetX];
            }
            buffer[x] = sum;
        }

        // Handle edge cases for x dimension;
        for (size_t x = 0; x < halfSize; x++)
        {
            T sumLeft = 0, sumRight = 0;
            for (int kernelOffset = -halfSize; kernelOffset <= halfSize; kernelOffset++)
            {
                int offsetLeftX = x + kernelOffset;
                int offsetRightX = offsetLeftX + _width - halfSize;
                sumLeft += kernelX[kernelOffset + halfSize] * GetPixel(offsetLeftX, y, CLAMP);
                sumRight += kernelX[kernelOffset + halfSize] * GetPixel(offsetRightX, y, CLAMP);
            }
            buffer[x] = sumLeft;
            buffer[x + _width - halfSize] = sumRight;
        }

        for (size_t i = 0; i < _width; i++)
        {
            _data[y * _width + i] = buffer[i];
        }
    }

    for (size_t x = 0; x < _width; x++)
    {
        // Handle general case.
        for (size_t y = halfSize; y < _height - halfSize; y++)
        {
            T sum = 0;
            for (int kernelOffset = -halfSize; kernelOffset <= halfSize; kernelOffset++)
            {
                int offsetY = y + kernelOffset;
                sum += kernelY[kernelOffset + halfSize] * _data[offsetY * _width + x];
            }
            buffer[y] = sum;
        }

        // Handle edge case for y dimension.
        for (size_t y = 0; y < halfSize; y++)
        {
            T sumTop = 0, sumBottom = 0;
            for (int kernelOffset = -halfSize; kernelOffset <= halfSize; kernelOffset++)
            {
                int offsetYTop = y + kernelOffset;
                int offsetYBottom = offsetYTop + _height - halfSize;
                sumTop += kernelY[kernelOffset + halfSize] * GetPixel(x, offsetYTop, CLAMP);
                sumBottom += kernelY[kernelOffset + halfSize] * GetPixel(x, offsetYBottom, CLAMP);
            }
            buffer[y] = sumTop;
            buffer[y + _height - halfSize] = sumBottom;
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
template class Image<unsigned char>;