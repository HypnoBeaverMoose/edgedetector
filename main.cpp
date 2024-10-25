#include <iostream>
#include "edge-detector.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

int main(int argc, char **argv)
{
    std::string filename;
    float threshold;

    if(argc != 3)
    {
        std::cout<<"Usage: edgedetector <filename> <threshold[0-1]>"<<std::endl;
        exit(1);
    }
    else
    {
        filename = argv[1];
        threshold = std::atof(argv[2]);
    }
    Image<float> input = TgaImageFactory(filename).GetImage();

    EdgeDetector<float> edgeDetector(true);
    Image<float> result = edgeDetector.FindEdges(input, threshold, 1.0f, 2);

    auto edgePixels = result.FindNonZeroPixels();
    for (auto &pixel : edgePixels)
    {
        std::cout << std::get<0>(pixel) << " " << std::get<1>(pixel) << std::endl;
    }
}
