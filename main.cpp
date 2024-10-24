#include <iostream>
#include <vector>
#include "kernels.hpp"
#include "image.hpp"
#include "processing.hpp"
#include "image-factory.hpp"


int main(int argc, char **argv)
{
    Image<int> im = PredefinedImageFactory().GetImage();

    Image<int> resultX = im.GetConvolved(3, sobelX);
    Image<int> resultY = im.GetConvolved(3, sobelY);
    Image<int> gradient = Image<int>::CombineImages(resultX, resultY, CombineGradients<int>);
    Image<int> direction = Image<int>::CombineImages(resultX, resultY, GradientDirection<int>);
    Image<int> result = FindLocalMaxima(gradient, direction);
    result.ApplyThreshold(10, 9);

    std::cout << gradient;
    std::cout << "-------" << std::endl;
    std::cout << direction;
    std::cout << "-------" << std::endl;
    std::cout << result;
}
