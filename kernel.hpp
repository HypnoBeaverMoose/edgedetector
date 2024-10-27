#pragma once
#include <vector>
#include <algorithm>

/// @brief Represents a separable kernel
/// @tparam T element type
/// @tparam N size
template <typename T, unsigned int N>
class Kernel
{
public:
    Kernel(const T horizontal[N], const T vertical[N]) : _horizontal(horizontal, horizontal + N), _vertical(vertical, vertical + N) {}

    const std::vector<T> &GetHorizontal() { return _horizontal; }

    const std::vector<T> &GetGetVertical() { return _vertical; }

    int GetSize() { return N; }

protected:
    std::vector<T> _horizontal;
    std::vector<T> _vertical;
};

/// @brief Represents a gaussian kernel
template <unsigned int N>
class Gaussian : public Kernel<float, N>
{
public:
    Gaussian(float sigma) : Kernel<float, N>((float[N]){}, (float[N]){})
    {
        float sigmaSqr = sigma * sigma;
        float div = 1.0f / std::sqrt(2 * M_PI * sigmaSqr);
        int size = N;
        int halfsize = size / 2;

        float sum = 0;
        for (int x = -halfsize; x <= halfsize; x++)
        {
            float val = std::exp(-0.5f * x * x / sigmaSqr) * div;
            this->_horizontal[x + halfsize] = val;
            sum += val;
        }

        for (auto &&element : this->_horizontal)
        {
            element /= sum;
        }

        std::copy(this->_horizontal.begin(), this->_horizontal.end(), this->_vertical.begin());
    }
};