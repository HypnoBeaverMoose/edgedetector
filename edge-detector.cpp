#include "image.hpp"
#include "edge-detector.hpp"
#include "kernel.hpp"
#include "file-utils.hpp"
#include <algorithm>
#include <iostream>
#include "auxiliary.hpp"

const float blurStrength = 1;

Gaussian<float, 5> gaussian(blurStrength);

float sobelXData1[3] = {1, 0, -1};
float sobelXData2[3] = {1, 2, 1};
Kernel<float, 3> SobelX(sobelXData1, sobelXData2);

template <typename T>
void EdgeDetector<T>::FindEdges(Image<T> &image, T threshold, T edgeStrength) const
{
    auto blurStart = std::chrono::high_resolution_clock::now();
    if (_useBlur)
    {
        // Blur the input image to remove noise.
        ApplyBlur(image);
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - blurStart);
    std::cout << "Time to apply blur: " << elapsed.count() << std::endl;

    if (_debug)
    {
        FileUtils::SaveImage(image, _debugPath + "blurred.tga");
    }

    // Try to reuse images to reduce memory allocation.
    Image<T> &filterX = image;
    Image<T> &initialEdges = image;
    Image<T> filterY(image);

    auto filterStart = std::chrono::high_resolution_clock::now();

    // Apply a filter in two directions to find the image gradients.
    // filterX is reused here.
    ApplyFilter(filterX, filterY);

    auto filterEnd = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(filterEnd - filterStart);
    std::cout << "Time to filter: " << elapsed.count() << std::endl;

    // Use the filtered images to find local maxima in the gradient.
    // Those correspond to best place for an edge.
    FindInitialEdges(filterX, filterY, initialEdges);

    auto initialEdgesEnd = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(initialEdgesEnd - filterEnd);
    std::cout << "Time to find initial edges: " << elapsed.count() << std::endl;
    ;

    if (_debug)
    {
        FileUtils::SaveImage(initialEdges, _debugPath + "initial-edges.tga");
    }

    auto doubleThresholdStart = std::chrono::high_resolution_clock::now();

    // Apply a LOW and a HIGH threshold - everything below LOW is set to zero.
    // Everything above HIGH is edgeStrength
    initialEdges.ApplyDoubleThreshold(threshold / 4, threshold, edgeStrength);

    auto doubleThresholdEnd = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(doubleThresholdEnd - doubleThresholdStart);
    std::cout << "Time to apply double thresold: " << elapsed.count() << std::endl;

    if (_debug)
    {
        FileUtils::SaveImage(initialEdges, _debugPath + "threshold.tga");
    }

    auto hysteresisStart = std::chrono::high_resolution_clock::now();
    // Hysteresis edge tracking - look for adjacent "strong" edges.
    TrackEdges(initialEdges, image, edgeStrength);

    auto hysteresisEnd = std::chrono::high_resolution_clock::now();
    elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(hysteresisEnd - hysteresisStart);
    std::cout << "Hysteresis: " << elapsed.count() << std::endl;

    if (_debug)
    {
        FileUtils::SaveImage(initialEdges, _debugPath + "final.tga");
    }
}

template <typename T>
void EdgeDetector<T>::ApplyBlur(Image<T> &input) const
{
    return input.Convolve(gaussian.GetHorizontal(), gaussian.GetGetVertical());
}

template <typename T>
void EdgeDetector<T>::ApplyFilter(Image<T> &filterX, Image<T> &filterY) const
{
    filterX.Convolve(SobelX.GetHorizontal(), SobelX.GetGetVertical());
    filterY.Convolve(SobelY.GetHorizontal(), SobelY.GetGetVertical());
}

template <typename T>
void EdgeDetector<T>::FindInitialEdges(const Image<T> &filterX, const Image<T> &filterY, Image<T> &result) const
{
    Image<T> gradient(filterX.GetWidth(), filterY.GetHeight());

    Image<unsigned char> direction(filterX.GetWidth(), filterY.GetHeight());

    // Add gradients in two directions to find gradient intensity.
    CombineImages<T, T, T>(filterX, filterY, gradient, Aux::CombineGradients<T, T, T>);

    // For each pixel, determine the direction of the gradient, and quantify that into 8 buckets.
    CombineImages<unsigned char, T, T>(filterX, filterY, direction, Aux::FindGradientDirection<unsigned char, T, T>);

    if (_debug)
    {
        FileUtils::SaveImage(gradient, _debugPath + "gradient.tga");
    }

    FindLocalMaxima(gradient, direction, result);
}

template <typename T>
void EdgeDetector<T>::FindLocalMaxima(const Image<T> &gradient, const Image<unsigned char> &direction, Image<T> &result) const
{
    if (gradient.GetHeight() != direction.GetHeight() || gradient.GetWidth() != direction.GetWidth())
    {
        std::cout << "Error: Gradient and Direction images of different dimensions.";
        exit(1);
    }

    // Should be understood in pairs.
    // Relative directions of adjacent pixels.
    static const int directions[8] = {0, 1, 1, 1, 1, 0, 1, -1};

    size_t width = (size_t)gradient.GetWidth();
    size_t height = (size_t)gradient.GetHeight();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // The direction index of the gradient is used both in the positive and negative directions.
            unsigned char directionIndex = direction.GetPixelUnsafe(x, y);
            int positiveX = x + directions[directionIndex * 2];
            int positiveY = y + directions[directionIndex * 2 + 1];

            int negativeX = x - directions[directionIndex * 2];
            int negativeY = y - directions[directionIndex * 2 + 1];

            // Find pixels in both gradient directions - in the directions of increase and decrease.
            T before = gradient.GetPixel(positiveX, positiveY);
            T after = gradient.GetPixel(negativeX, negativeY);
            T current = gradient.GetPixelUnsafe(x, y);

            // If neighbouring pixels have less intensity we have found a local maximum.
            // We want to keep the local maxima, and set all other pixels to zero.
            // >= ensures we keep one of maximum pixels with the same intensity.
            if (current >= before && current > after)
            {
                result.SetPixel(x, y, current);
            }
            else
            {
                result.SetPixel(x, y, 0);
            }
        }
    }
}

template <typename T>
void EdgeDetector<T>::TrackEdges(const Image<T> &input, Image<T> &result, T edgeValue) const
{
    int width = input.GetWidth();
    int height = input.GetHeight();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            result.SetPixel(x, y, DetermineEdgeValue(input, x, y, edgeValue));
        }
    }
}

template <typename T>
T EdgeDetector<T>::DetermineEdgeValue(const Image<T> &input, int x, int y, T edgeValue) const
{
    T val = input.GetPixelUnsafe(x, y);
    if (!(val > 0 && val < edgeValue))
    {
        return val;
    }

    static const int connectivity[16] = {-1, -1, 0, -1, 1, -1, -1, 0, 1, 0, -1, 1, 0, 1, 1, 1};

    for (size_t i = 0; i < 8; i++)
    {
        // If there are any neighbouring pixels with high intensity, this pixel is part of a strong edge and should be kept.
        int realX = x + connectivity[i * 2];
        int realY = y + connectivity[i * 2 + 1];
        T pixel = input.GetPixel(realX, realY, Image<T>::CLAMP);
        if (std::abs(pixel - edgeValue) < std::numeric_limits<T>::epsilon())
        {
            return edgeValue;
        }
    }

    return 0;
}

template class EdgeDetector<float>;
