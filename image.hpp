#pragma once
#include <vector>
#include <iostream>

template <typename T>
class Image
{
public:
    enum OverflowStrategy
    {
        DEFAULT,
        CLAMP,
        MIRROR
    };

    Image() : _width(0), _height(0) {}

    Image(int width, int height) : _width(width), _height(height), _data(width * height) {};

    Image(int width, int height, const std::vector<T> &input) : _width(width), _height(height), _data(input) {}

    Image(int width, int height, const std::vector<T> &&input) : _width(width), _height(height), _data(input) {}

    T GetPixel(int x, int y, OverflowStrategy overflow = DEFAULT, T def = 0) const;

    T GetPixelUnsafe(int x, int y) const { return _data[y * _width + x]; }

    void SetPixelUnsafe(int x, int y, T value) { _data[y * _width + x] = value; }

    void SetPixel(int x, int y, T value);

    int GetWidth() const;

    int GetHeight() const;

    void Convolve(const std::vector<T> &kernelX, const std::vector<T> &kernelY);

    Image<T> GetConvolved(const std::vector<T> &kernel) const;

    Image<T> GetConvolvedSeparable(const std::vector<T> &kernelX, const std::vector<T> &kernelY) const;

    std::vector<std::tuple<int, int>> FindNonZeroPixels() const;

    void Normalize(T maximum);

    void ApplyDoubleThreshold(T low, T high, T max);

    template <typename TRet, typename TLeft, typename TRight>
    friend void CombineImages(const Image<TLeft> &left, const Image<TRight> &right, Image<TRet> &result, std::function<TRet(TLeft, TRight)> func);

private:
    T NormalizeElement(T element, T normalizer);

    std::vector<T> _data;
    int _width;
    int _height;
};

template <typename TRet, typename TLeft, typename TRight>
void CombineImages(const Image<TLeft> &left, const Image<TRight> &right, Image<TRet> &result, std::function<TRet(TLeft, TRight)> func)
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
std::ostream &operator<<(std::ostream &strm, const Image<T> &a)
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