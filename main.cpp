#include <iostream>
#include "kernel.hpp"
#include "edge-detector.hpp"
#include "image-factory.hpp"
#include "file-utils.hpp"

void PrintNonZeroPixels(std::string filename, float threshold);
void ConvolveTest(std::string filename);

void VectorFunction(std::vector<int> in)
{
    in[0] = 10;
}

void VectorTest()
{
    std::vector<int> test = {1, 2, 3, 4, 5};
    VectorFunction(test);
    for (auto &&i : test)
    {
        std::cout << i << " ";
    }
}

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
}

void PrintNonZeroPixels(std::string filename, float threshold)
{
    Image<float> image = TgaImageFactory(filename).GetImage();

    EdgeDetector<float> edgeDetector(true);
    edgeDetector.FindEdges(image, threshold, 1.0f, 2);

    auto edgePixels = image.FindNonZeroPixels();
    for (auto &pixel : edgePixels)
    {
        std::cout << std::get<0>(pixel) << " " << std::get<1>(pixel) << std::endl;
    }
}

void ConvolveTest(std::string filename)
{
    Gaussian<5> Gaussian5(2);
    Image<float> input = TgaImageFactory(filename).GetImage();

    auto start = std::chrono::system_clock::now();
    auto result = input.GetConvolvedSeparable(Gaussian5.GetHorizontal(), Gaussian5.GetGetVertical());
    auto end1 = std::chrono::system_clock::now();

    input.Convolve(Gaussian5.GetHorizontal(), Gaussian5.GetGetVertical());
    auto end2 = std::chrono::system_clock::now();

    std::cout << (end1 - start).count() << " " << (end2 - end1).count() << '\n';

    FileUtils::SaveImage(input, "result.tga");
}
