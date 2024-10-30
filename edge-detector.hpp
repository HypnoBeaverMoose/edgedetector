#pragma once
#include <string>
#include "image.hpp"


/// @brief Performs Canny edge detection.
template <typename T>
class EdgeDetector
{
public:
    EdgeDetector(bool useBlur, bool debug = false, std::string debugPath = "images/debug-images/") : _debugPath(debugPath), _debug(debug), _useBlur(useBlur) {};

    /// @brief Finds edges of features defined in image and writes them in that same image.
    /// @param image input image.
    /// @param threshold threshold below, which edges are discarded.
    /// @param edgeStrength maximum edge strength.
    void FindEdges(Image<T> &input, T threshold, T edgeStrength) const;

private:
    void ApplyBlur(Image<T> &input) const;

    void ApplyFilter(Image<T> &filterX, Image<T> &filterY) const;

    void FindInitialEdges(const Image<T> &filterX, const Image<T> &filterY, Image<T> &result) const;

    void FindLocalMaxima(const Image<T> &gradient, const Image<unsigned char> &direction, Image<T> &result) const;

    void TrackEdges(const Image<T> &input, Image<T> &result, T edgeValue) const;

    T DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue) const;

    bool _useBlur;
    std::string _debugPath;
    bool _debug;
};