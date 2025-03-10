#include <iostream>
#include "kernel.hpp"
#include "edge-detector.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

void FindEdges(std::string input, std::string output, float threshold);

int main(int argc, char **argv)
{
    std::string input, output;
    float threshold;

    if (argc != 4)
    {
        std::cout << "Usage: edgedetector <input> <output> <threshold[0-1]>" << std::endl;
        exit(1);
    }
    else
    {
        input = argv[1];
        output = argv[2];
        threshold = std::atof(argv[3]);
    }

    FindEdges(input, output, threshold);

    return 0;
}

void FindEdges(std::string inputFilename, std::string outputFilename, float threshold)
{
    Image<float> image = TgaImageFactory(inputFilename).GetImage();

    EdgeDetector<float> edgeDetector(false, true, "");

    edgeDetector.FindEdges(image, threshold, 1.0f);

    FileUtils::SaveImage(image, outputFilename);
}
