#include "image.hpp"
#include "edge-detector.hpp"
#include "kernel.hpp"
#include "file-utils.hpp"
#include <algorithm>
#include <iostream>

int offsets[4][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}};

Kernel<float, 3> SobelX((float[3]){1, 0, -1}, (float[3]){1, 2, 1});
Kernel<float, 3> SobelY((float[3]){1, 2, 1}, (float[3]){1, 0, -1});


template <typename T>
T FindGradientDirection(T left, T right)
{
    T deg = (std::atan2(left, right) * 57.2958);
    deg = deg / 45.0f;

    int index = std::round(deg);
    if (index < 0)
    {
        index += 4;
    }
    else if (index == 4)
    {
        index = 0;
    }
    return (T)index;
}

template <typename T>
T CombineGradients(T left, T right)
{
    return std::sqrt(left * left + right * right);
}

template <typename T>
T Difference(T left, T right)
{
    return std::abs(left - right);
}

template <typename T>
Image<T> EdgeDetector<T>::FindEdges(const Image<T> &input, T threshold, T edgeStrength, float blurStrength) const
{
    Image<T> blurred = ApplyBlur(input, blurStrength);
    if (_debug)
    {
        FileUtils::SaveImage(blurred, _debugPath + "blurred.tga");
    }

    std::tuple<Image<T>, Image<T>> filtered = ApplyFilter(blurred);

    Image<T> result = FindInitialEdges(std::get<0>(filtered), std::get<1>(filtered));

    if (_debug)
    {
        FileUtils::SaveImage(result, _debugPath + "initial-edges.tga");
    }

    result.ApplyDoubleThreshold(threshold / 4, threshold, edgeStrength);

    if (_debug)
    {
        FileUtils::SaveImage(result, _debugPath + "threshold.tga");
    }

    result = TrackEdges(result, edgeStrength);

    if (_debug)
    {
        FileUtils::SaveImage(result, _debugPath + "final.tga");
    }

    return result;
}

template <typename T>
Image<T> EdgeDetector<T>::FindInitialEdges(const Image<T> &resultX, const Image<T> &resultY) const
{
    Image<T> gradient = Image<float>::CombineImages(resultX, resultY, CombineGradients<T>);

    if (_debug)
    {
        FileUtils::SaveImage(gradient, _debugPath + "gradient.tga");
    }

    Image<T> direction = Image<float>::CombineImages(resultX, resultY, FindGradientDirection<T>);

    if (_debug)
    {
        FileUtils::SaveImage(direction, _debugPath + "direction.tga");
    }

    return FindLocalMaxima(gradient, direction);
}

template <typename T>
std::tuple<Image<T>, Image<T>> EdgeDetector<T>::ApplyFilter(const Image<T> &input) const
{
    Image<T> resultX = input.GetConvolvedSeparable(SobelX.GetHorizontal(), SobelX.GetGetVertical());
    Image<T> resultY = input.GetConvolvedSeparable(SobelY.GetHorizontal(), SobelY.GetGetVertical());
    return {resultX, resultY};
}

template <typename T>
Image<T> EdgeDetector<T>::ApplyBlur(const Image<T> &input, float blurStrength) const
{
    Gaussian<10> gaussian(blurStrength);
    return input.GetConvolvedSeparable(gaussian.GetHorizontal(), gaussian.GetGetVertical());
}

template <typename T>
Image<T> EdgeDetector<T>::FindLocalMaxima(const Image<T> &gradient, const Image<T> &direction) const
{
    if (gradient.GetHeight() != direction.GetHeight() || gradient.GetWidth() != direction.GetWidth())
    {
        std::cout << "Error: Gradient and Direction images of different dimensions.";
        exit(1);
    }

    int width = gradient.GetWidth();
    int height = gradient.GetHeight();

    Image<T> result(width, height);

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            int directionIndex = (int)direction.GetPixel(x, y);
            int positiveX = x + offsets[directionIndex][0];
            int positiveY = y + offsets[directionIndex][1];

            int negativeX = x - offsets[directionIndex][0];
            int negativeY = y - offsets[directionIndex][1];

            T before = gradient.GetPixel(positiveX, positiveY);
            T after = gradient.GetPixel(negativeX, negativeY);
            T current = gradient.GetPixel(x, y);

            if (current >= before && current > after)
            {
                result.SetPixel(x, y, current);
            }
            else
            {
                result.SetPixel(x, y, 0);
            }
        }
    }
    return result;
}

template <typename T>
Image<T> EdgeDetector<T>::TrackEdges(const Image<T> &input, T edgeValue) const
{
    int width = input.GetWidth();
    int height = input.GetHeight();

    Image<T> result(width, height);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            result.SetPixel(x, y, DetermineEdgeValue(input, x, y, edgeValue));
        }
    }
    return result;
}

template <typename T>
T EdgeDetector<T>::DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue) const
{
    T val = input.GetPixel(x, y, Image<T>::CLAMP);
    if (!(val > 0 && val < edgeValue))
    {
        return val;
    }

    for (int offset_y = -1; offset_y <= 1; offset_y++)
    {
        for (int offset_x = -1; offset_x <= 1; offset_x++)
        {
            int realX = x + offset_x;
            int realY = y + offset_y;
            if (offset_x > 0 && offset_y > 0)
            {
                T pixel = input.GetPixel(realX, realY, Image<T>::CLAMP);
                if (std::abs(pixel - edgeValue) < std::numeric_limits<T>::epsilon())
                {
                    return edgeValue;
                }
            }
        }
    }

    return 0;
}

template class EdgeDetector<float>;