#include <iostream>
#include <vector>
#include "kernels.hpp"
#include "image.hpp"
#include "image-factory.hpp"

int GradientDirection(int left, int right)
{
    //return std::round((std::atan2((float)left, (float)right) * 57.2958f));
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

int main(int argc, char **argv)
{
    Image im = PredefinedImageFactory().GetImage();

    Image resultX = im.Convolve(3, sobelX);
    Image resultY = im.Convolve(3, sobelY);
    Image gradient = Image::CombineImages(resultX, resultY, CombineGradients);
    Image direction = Image::CombineImages(resultX, resultY, GradientDirection);

    std::cout << gradient;
    std::cout << "-------" << std::endl;
    std::cout << direction;
}
