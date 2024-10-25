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
    Image<float> blurred = im.GetConvolvedSeparable(Gaussian5.GetHorizontal(), Gaussian5.GetGetVertical());
    Image<float> resultX = blurred.GetConvolvedSeparable(SobelX.GetHorizontal(), SobelX.GetGetVertical());
    Image<float> resultY = blurred.GetConvolvedSeparable(SobelY.GetHorizontal(), SobelY.GetGetVertical());

    Image<float> gradient = Image<float>::CombineImages(resultX, resultY, CombineGradients<float>);
    Image<float> direction = Image<float>::CombineImages(resultX, resultY, GradientDirection<float>);
    Image<float> result = FindLocalMaxima(gradient, direction);
    result.ApplyDoubleThreshold(0.05f, 0.1f, 1.0f);

    // // gradient.Normalize(1.0f);
    FileUtils::SaveImage(result, "output.tga");
}
