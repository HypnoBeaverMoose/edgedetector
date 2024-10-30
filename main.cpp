#include <iostream>
#include "kernel.hpp"
#include "edge-detector.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

void PrintNonZeroPixels(std::string filename, float threshold);

int main(int argc, char **argv)
{
    std::string filename;
    float threshold;

    if (argc != 3)
    {
        std::cout << "Usage: edgedetector <filename> <threshold[0-1]>" << std::endl;
        exit(1);
    }
    else
    {
        filename = argv[1];
        threshold = std::atof(argv[2]);
    }

    PrintNonZeroPixels(filename, threshold);

    return 0;
}

void PrintNonZeroPixels(std::string filename, float threshold)
{
    Image<float> image = TgaImageFactory(filename).GetImage();

    EdgeDetector<float> edgeDetector(false, true, "");

    edgeDetector.FindEdges(image, threshold, 1.0f);

    auto edgePixels = image.FindNonZeroPixels();
    for (auto &pixel : edgePixels)
    {
        std::cout << std::get<0>(pixel) << " " << std::get<1>(pixel) << std::endl;
    }
}