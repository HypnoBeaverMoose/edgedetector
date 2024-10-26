#include "image.hpp"
#include "edge-detector.hpp"
#include "kernel.hpp"
#include "file-utils.hpp"
#include <algorithm>
#include <iostream>

int offsets[8] = {0, 1, 1, 1, 1, 0, 1, -1};
int connectivity[16] = {-1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1};

Kernel<float, 3>
    SobelX((float[3]){1, 0, -1}, (float[3]){1, 2, 1});
Kernel<float, 3> SobelY((float[3]){1, 2, 1}, (float[3]){1, 0, -1});

template <typename TRet, typename TLeft, typename TRight>
TRet FindGradientDirection(TLeft left, TRight right)
{
    TLeft deg = (std::atan2(left, right) * 57.2958);
    deg = deg / 45;

    int index = std::round(deg);
    if (index < 0)
    {
        index += 4;
    }
    else if (index == 4)
    {
        index = 0;
    }
    return index;
}

template <typename TRet, typename TLeft, typename TRight>
TRet CombineGradients(TLeft left, TRight right)
{
    return std::sqrt(left * left + right * right);
}

template <typename TRet, typename TLeft, typename TRight>
TRet Difference(TLeft left, TRight right)
{
    return std::abs(left - right);
}

template <typename T>
void EdgeDetector<T>::FindEdges(Image<T> &image, T threshold, T edgeStrength, float blurStrength) const
{
    ApplyBlur(image, blurStrength);
    if (_debug)
    {
        FileUtils::SaveImage(image, _debugPath + "blurred.tga");
    }

    Image<T> &filterX = image;
    Image<T> &initialEdges = image;
    Image<T> filterY(image);

    ApplyFilter(filterX, filterY);
    FindInitialEdges(filterX, filterY, initialEdges);

    if (_debug)
    {
        FileUtils::SaveImage(initialEdges, _debugPath + "initial-edges.tga");
    }

    initialEdges.ApplyDoubleThreshold(threshold / 4, threshold, edgeStrength);

    if (_debug)
    {
        FileUtils::SaveImage(initialEdges, _debugPath + "threshold.tga");
    }

    TrackEdges(initialEdges, image, edgeStrength);

    if (_debug)
    {
        FileUtils::SaveImage(initialEdges, _debugPath + "final.tga");
    }
}

template <typename T>
void EdgeDetector<T>::FindInitialEdges(const Image<T> &filterX, const Image<T> &filterY, Image<T> &result) const
{
    Image<T> gradient(filterX.GetWidth(), filterY.GetHeight());
    Image<unsigned char> direction(filterX.GetWidth(), filterY.GetHeight());

    CombineImages<T, T, T>(filterX, filterY, gradient, CombineGradients<T, T, T>);
    CombineImages<unsigned char, T, T>(filterX, filterY, direction, FindGradientDirection<unsigned char, T, T>);

    if (_debug)
    {
        FileUtils::SaveImage(gradient, _debugPath + "gradient.tga");
    }

    FindLocalMaxima(gradient, direction, result);
}

template <typename T>
void EdgeDetector<T>::ApplyFilter(Image<T> &filterX, Image<T> &filterY) const
{
    filterX.Convolve(SobelX.GetHorizontal(), SobelX.GetGetVertical());
    filterY.Convolve(SobelY.GetHorizontal(), SobelY.GetGetVertical());
}

template <typename T>
void EdgeDetector<T>::ApplyBlur(Image<T> &input, float blurStrength) const
{
    Gaussian<5> gaussian(blurStrength);
    return input.Convolve(gaussian.GetHorizontal(), gaussian.GetGetVertical());
}

template <typename T>
void EdgeDetector<T>::FindLocalMaxima(const Image<T> &gradient, const Image<unsigned char> &direction, Image<T> &result) const
{
    if (gradient.GetHeight() != direction.GetHeight() || gradient.GetWidth() != direction.GetWidth())
    {
        std::cout << "Error: Gradient and Direction images of different dimensions.";
        exit(1);
    }

    int width = gradient.GetWidth();
    int height = gradient.GetHeight();

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            unsigned char directionIndex = direction.GetPixelUnsafe(x, y);
            int positiveX = x + offsets[directionIndex * 2];
            int positiveY = y + offsets[directionIndex * 2 + 1];

            int negativeX = x - offsets[directionIndex * 2];
            int negativeY = y - offsets[directionIndex * 2 + 1];

            T before = gradient.GetPixel(positiveX, positiveY);
            T after = gradient.GetPixel(negativeX, negativeY);

            T current = gradient.GetPixelUnsafe(x, y);

            if (current >= before && current > after)
            {
                result.SetPixelUnsafe(x, y, current);
            }
            else
            {
                result.SetPixelUnsafe(x, y, 0);
            }
        }
    }
}

template <typename T>
void EdgeDetector<T>::TrackEdges(const Image<T> &input, Image<T> &result, T edgeValue) const
{
    int width = input.GetWidth();
    int height = input.GetHeight();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            result.SetPixelUnsafe(x, y, DetermineEdgeValue(input, x, y, edgeValue));
        }
    }
}

template <typename T>
T EdgeDetector<T>::DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue) const
{
    T val = input.GetPixelUnsafe(x, y);
    if (!(val > 0 && val < edgeValue))
    {
        return val;
    }

    for (size_t i = 0; i < 8; i++)
    {
        int realX = x + connectivity[i * 2];
        int realY = y + connectivity[i * 2 + 1];
        T pixel = input.GetPixel(realX, realY, Image<T>::CLAMP);
        if (std::abs(pixel - edgeValue) < std::numeric_limits<T>::epsilon())
        {
            return edgeValue;
        }
    }

    return 0;
}

template class EdgeDetector<float>;