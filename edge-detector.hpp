#pragma once
#include <string>
#include "image.hpp"

template <typename T>
class EdgeDetector
{
public:
    EdgeDetector(bool debug = false, std::string debugPath = "images/debug-images/") : _debugPath(debugPath), _debug(debug) {};

    void FindEdges(Image<T> &input, T threshold, T edgeStrength, float blurStrength) const;

private:
    void ApplyBlur(Image<T> &input, float blurStrength) const;

    void ApplyFilter(Image<T> &filterX, Image<T> &filterY) const;

    void FindInitialEdges(const Image<T> &filterX, const Image<T> &filterY, Image<T> &result) const;

    void FindLocalMaxima(const Image<T> &gradient, const Image<unsigned char> &direction, Image<T> &result) const;

    void TrackEdges(const Image<T> &input, Image<T> &result, T edgeValue) const;

    T DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue) const;

    std::string _debugPath;
    bool _debug;
};