#include <iostream>
#include <vector>
#include "kernels.hpp"
#include "image.hpp"
#include "processing.hpp"
#include "image-factory.hpp"


int main(int argc, char **argv)
{
    Image im = PredefinedImageFactory().GetImage();

    Image resultX = im.Convolve(3, sobelX);
    Image resultY = im.Convolve(3, sobelY);
    Image gradient = Image::CombineImages(resultX, resultY, CombineGradients);
    Image direction = Image::CombineImages(resultX, resultY, GradientDirection);
    Image result = FindLocalMaxima(gradient, direction);

    std::cout << gradient;
    std::cout << "-------" << std::endl;
    std::cout << direction;
    std::cout << "-------" << std::endl;
    std::cout << result;
}
