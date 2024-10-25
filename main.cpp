#include <iostream>
#include "edge-detector.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

int main(int argc, char **argv)
{
    Image<float> input = TgaImageFactory("images/input.tga").GetImage();

    EdgeDetector<float> edgeDetector(true);
    Image<float> result = edgeDetector.FindEdges(input, 0.25f, 1.0f, 2);

    auto edgePixels = result.FindNonZeroPixels();
    for (auto &pixel : edgePixels)
    {
        std::cout << std::get<0>(pixel) << " " << std::get<1>(pixel) << std::endl;
    }
}
