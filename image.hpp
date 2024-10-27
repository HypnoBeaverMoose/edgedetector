#pragma once
#include <vector>
#include <iostream>

/// @brief Represents an image.
template <typename T>
class Image
{
public:
    /// @brief Used to determine how queries outside of image boundaries are handled.
    enum OverflowStrategy
    {
        DEFAULT,
        CLAMP,
        MIRROR
    };

    Image() : _width(0), _height(0) {}

    Image(int width, int height) : _width(width), _height(height), _data(width * height) {};

    Image(int width, int height, const std::vector<T> &input) : _width(width), _height(height), _data(input) {}

    Image(int width, int height, const std::vector<T> &&input) : _width(width), _height(height), _data(std::move(input)) {}

    /// @brief Returns a pixel.
    /// @param x x coord.
    /// @param y y coord.
    /// @param overflow Determines what happens when x or y are outside of image boundaries.
    /// @param def Default value to use with DEFAULT overflow strategy.
    T GetPixel(int x, int y, OverflowStrategy overflow = DEFAULT, T def = 0) const;

    /// @brief Returns a pixel without checking for overflow.
    T GetPixelUnsafe(int x, int y) const { return _data[y * _width + x]; }

    /// @brief Set's a pixel's value.
    void SetPixel(int x, int y, T value);

    int GetWidth() const { return _width; }

    int GetHeight() const { return _height; }

    /// @brief Executes a convolution on the image using a separable kernel.
    void Convolve(const std::vector<T> &kernelX, const std::vector<T> &kernelY);

    /// @brief Finds pixels of non-zero intensity.
    std::vector<std::tuple<int, int>> FindNonZeroPixels() const;

    /// @brief Normalizes pixel values based on maximum value.
    /// @param maximum
    void Normalize(T maximum);

    /// @brief Applies a double threshold algorithm. Pixels below low a set to 0, above high are set to max
    void ApplyDoubleThreshold(T low, T high, T max);

    template <typename TRet, typename TLeft, typename TRight>
    friend void CombineImages(const Image<TLeft> &left, const Image<TRight> &right, Image<TRet> &result, std::function<TRet(TLeft, TRight)> func);

private:
    T NormalizeElement(T element, T normalizer);

    std::vector<T> _data;
    int _width;
    int _height;
};

/// @brief Combines two images, by executing a function for each pixel and records the result in a third image.
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