#include "file-utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>

#pragma pack(push, 1) // TODO::Will this work on other compilers?
struct Header
{
    char idLength;
    char colorMapType;
    char imageType;
    short colorMapOrigin;
    short colorMapOrigin2;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char pixelDepth;
    char imageDescriptor;
};
#pragma pack(pop)

Image<float> FileUtils::LoadImage(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }

    Header header;
    std::cout << sizeof(Header);
    // Read header
    file.read(reinterpret_cast<char *>(&header), sizeof(header));
    if (!file)
    {
        std::cerr << "Error reading header" << std::endl;
        exit(1);
    }
    std::vector<unsigned char> data(header.width * header.height * (header.pixelDepth / 8));
    file.read(reinterpret_cast<char *>(data.data()), data.size());
    if (!file)
    {
        std::cerr << "Error reading data" << std::endl;
        exit(1);
    }

    std::vector<float> result;
    int pixelCount = data.size() / 3;
    for (size_t i = 0; i < pixelCount; i++)
    {
        int channel = i * 3;
        float intensity = ((float)data[channel] +
                           (float)data[channel + 1] + (float)data[channel + 2]) /
                          (3.0f * 255);

        result.push_back(intensity);
    }

    return Image<float>(header.width, header.height, result);
}

void FileUtils::SaveImage(Image<float> image, std::string filename)
{
    std::vector<unsigned char> data;
    for (size_t y = 0; y < image.GetHeight(); y++)
    {
        for (size_t x = 0; x < image.GetWidth(); x++)
        {
            unsigned char pixel = (unsigned char)(image.GetPixel(x, y) * 255);
            data.push_back(pixel);
            data.push_back(pixel);
            data.push_back(pixel);
        }
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        file.close();
        exit(1);
    }
    Header header = {0};
    header.imageType = 2;
    header.width = image.GetWidth();
    header.height = image.GetHeight();
    header.pixelDepth = 24;

    // Write header
    file.write(reinterpret_cast<const char *>(&header), sizeof(header));
    if (!file)
    {
        std::cerr << "Error writing header" << std::endl;
        file.close();
        exit(1);
    }

    // Write pixel data
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
    if (!file)
    {
        std::cerr << "Error writing pixel data" << std::endl;
        file.close();
        exit(1);
    }
}
