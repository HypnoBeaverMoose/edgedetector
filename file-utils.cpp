#include "file-utils.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

Image<float> FileUtils::LoadImage(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }

    unsigned char header[18] = {0};
    // Read header
    file.read((char *)header, sizeof(header));
    if (!file)
    {
        std::cerr << "Error reading header" << std::endl;
        exit(1);
    }

    int width = (header[13] << 8) + header[12];
    int height = (header[15] << 8) + header[14];
    int channels = header[16] / 8;

    std::vector<unsigned char> data(width * height * channels);
    file.read((char *)data.data(), data.size());
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
                          (channels * 255.0f);

        result.push_back(intensity);
    }

    file.close();
    return Image<float>(width, height, std::move(result));
}

void FileUtils::SaveImage(Image<float> image, std::string filename)
{
    std::vector<unsigned char> data;
    for (size_t y = 0; y < image.GetHeight(); y++)
    {
        for (size_t x = 0; x < image.GetWidth(); x++)
        {
            unsigned char pixel = (unsigned char)(image.GetPixelUnsafe(x, y) * 255);
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

    unsigned char header[18] = {0};
    header[2] = 2;
    header[12] = image.GetWidth() & 0xFF;
    header[13] = (image.GetWidth() >> 8) & 0xFF;
    header[14] = image.GetHeight() & 0xFF;
    header[15] = (image.GetHeight() >> 8) & 0xFF;
    header[16] = 24;

    // Write header
    file.write((char *)header, sizeof(header));
    if (!file)
    {
        std::cerr << "Error writing header" << std::endl;
        file.close();
        exit(1);
    }

    // Write pixel data
    file.write((char *)(data.data()), data.size());
    if (!file)
    {
        std::cerr << "Error writing pixel data" << std::endl;
        file.close();
        exit(1);
    }

    file.close();
}
