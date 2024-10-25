#pragma once
#include "image.hpp"
#include <algorithm>
#include <iostream>

struct Coord
{
    Coord(int x, int y) : X(x), Y(y) {}

    int X;
    int Y;
};

int offsets[4][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}};

template <typename T>
T GradientDirection(T left, T right)
{
    float deg = (std::atan2((float)left, (float)right) * 57.2958);
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
    return index;
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
std::vector<Coord> FindNonZeroPixels(const Image<T> &image)
{
    std::vector<Coord> result;
    for (size_t y = 0; y < image.GetHeight(); y++)
    {
        for (size_t x = 0; x < image.GetWidth(); x++)
        {
            if (image.GetPixel(x, y) > 0)
            {
                result.push_back(Coord(x, y));
            }
        }
    }
    return result;
}

template <typename T>
Image<T> FindLocalMaxima(const Image<T> &gradient, const Image<T> &direction)
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
Image<T> EdgeTracking(const Image<T> &input, T edgeValue)
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
T DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue)
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