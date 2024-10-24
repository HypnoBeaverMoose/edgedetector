#pragma once
#include <vector>
#include <iostream>

class Image
{
public:
    enum OverflowStrategy
    {
        DEFAULT,
        CLAMP,
        MIRROR
    };

    Image() : _width(0), _height(0) {}

    Image(int width, int height) : _width(width), _height(height), _data(width * height) {};

    Image(int width, int height, const std::vector<int> &input);

    int GetPixel(int x, int y, OverflowStrategy overflow = DEFAULT, int def = 0) const;

    void SetPixel(int x, int y, int value);

    int GetWidth() const;

    int GetHeight() const;

    Image Convolve(int size, int *kernel);

    static Image CombineImages(const Image &left, const Image &right, std::function<int(int, int)> func);

private:
    std::vector<int> _data;
    int _width;
    int _height;
};

std::ostream &operator<<(std::ostream &strm, const Image &a);