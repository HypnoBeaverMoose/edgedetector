#pragma once
#include <vector>
#include <iostream>

class Image
{
public:
    Image(int width, int height, const std::vector<int> &input);

    int GetPixel(int x, int y) const;

    void SetPixel(int x, int y, int value);

    int GetWidth() const;

    int GetHeight() const;

    void Convolve(int size, int* kernel);

private:
    std::vector<int> _data;
    int _width;
    int _height;
};

std::ostream &operator<<(std::ostream &strm, const Image &a);