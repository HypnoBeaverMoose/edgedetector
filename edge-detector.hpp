#pragma once
#include <string>
#include "image.hpp"

template <typename T>
class EdgeDetector
{
public:
    EdgeDetector(std::string debugPath) : _debugPath(debugPath) {};

    Image<T> FindEdges(const Image<T> &input, T threshold, T edgeStrength, float blurStrength) const;

private:
    Image<T> ApplyBlur(const Image<T> &input, float blurStrength) const;

    std::tuple<Image<T>, Image<T>> ApplyFilter(const Image<T> &input) const;

    Image<T> FindInitialEdges(const Image<T> &gradient, const Image<T> &direction) const;

    Image<T> FindLocalMaxima(const Image<T> &gradient, const Image<T> &direction) const;

    Image<T> TrackEdges(const Image<T> &input, T edgeValue) const;

    T DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue) const;

    std::string _debugPath;
};