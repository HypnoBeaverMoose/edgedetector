#include <iostream>
#include <vector>
#include "kernels.hpp"
#include "image.hpp"
#include "processing.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

int main(int argc, char **argv)
{
    Image<int> im = TgaImageFactory("input.tga").GetImage();

    Image<int> resultX = im.GetConvolved(3, sobelX);
    Image<int> resultY = im.GetConvolved(3, sobelY);
    Image<int> gradient = Image<int>::CombineImages(resultX, resultY, CombineGradients<int>);
    Image<int> direction = Image<int>::CombineImages(resultX, resultY, GradientDirection<int>);
    Image<int> result = FindLocalMaxima(gradient, direction);
    result.ApplyThreshold(255, 30);

    FileUtils::SaveImage(result, "output.tga");
    // std::vector<Coord> nonZero = FindNonZeroPixels(result);
    // for (size_t i = 0; i < nonZero.size(); i++)
    // {
    //     std::cout << nonZero[i].X << " " << nonZero[i].Y << std::endl;
    // }
}
