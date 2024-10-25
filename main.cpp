#include <iostream>
#include <vector>
#include "kernel.hpp"
#include "image.hpp"
#include "processing.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

int main(int argc, char **argv)
{
    Image<float> im = TgaImageFactory("input.tga").GetImage();

    Image<float> resultX = im.GetConvolvedSeparable(SobelX.GetHorizontal(), SobelX.GetGetVertical());
    Image<float> resultY = im.GetConvolvedSeparable(SobelY.GetHorizontal(), SobelY.GetGetVertical());

    Image<float> gradient = Image<float>::CombineImages(resultX, resultY, CombineGradients<float>);
    Image<float> direction = Image<float>::CombineImages(resultX, resultY, GradientDirection<float>);
    Image<float> result = FindLocalMaxima(gradient, direction);
    result.ApplyThreshold(1, 0.12f);

    // gradient.Normalize(1.0f);
    FileUtils::SaveImage(result, "output.tga");
}
