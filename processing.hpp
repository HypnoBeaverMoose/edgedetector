#pragma once
#include "image.hpp"
#include <algorithm>
#include <iostream>

int offsets[4][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}};

int GradientDirection(int left, int right)
{
    // return std::round((std::atan2((float)left, (float)right) * 57.2958f));
    float deg = (std::atan2((float)left, (float)right) * 57.2958f);
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

int CombineGradients(int left, int right)
{
    return std::round(std::sqrt(left * left + right * right));
}

Image FindLocalMaxima(const Image &gradient, const Image &direction)
{
    if (gradient.GetHeight() != direction.GetHeight() || gradient.GetWidth() != direction.GetWidth())
    {
        std::cout << "Error: Gradient and Direction images of different dimensions.";
        exit(1);
    }

    int width = gradient.GetWidth();
    int height = gradient.GetHeight();

    Image result(width, height);

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            int directionIndex = direction.GetPixel(x, y);
            int positiveX = x + offsets[directionIndex][0];
            int positiveY = y + offsets[directionIndex][1];

            int negativeX = x - offsets[directionIndex][0];
            int negativeY = y - offsets[directionIndex][1];

            int before = gradient.GetPixel(positiveX, positiveY);
            int after = gradient.GetPixel(negativeX, negativeY);
            int current = gradient.GetPixel(x, y);

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