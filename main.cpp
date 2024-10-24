#include <iostream>
#include <vector>
#include "kernels.hpp"
#include "image.hpp"
#include "processing.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

int main(int argc, char **argv)
{
    Image<float> im = TgaImageFactory("input.tga").GetImage();

    Image<float> resultX = im.GetConvolved(3, sobelX);
    Image<float> resultY = im.GetConvolved(3, sobelY);
    Image<float> gradient = Image<float>::CombineImages(resultX, resultY, CombineGradients<float>);
    Image<float> direction = Image<float>::CombineImages(resultX, resultY, GradientDirection<float>);
    Image<float> result = FindLocalMaxima(gradient, direction);
    result.ApplyThreshold(1, 0.12f);

    //gradient.Normalize(1.0f);
    FileUtils::SaveImage(result, "output.tga");
    // std::vector<Coord> nonZero = FindNonZeroPixels(result);
    // for (size_t i = 0; i < nonZero.size(); i++)
    // {
    //     std::cout << nonZero[i].X << " " << nonZero[i].Y << std::endl;
    // }
}
