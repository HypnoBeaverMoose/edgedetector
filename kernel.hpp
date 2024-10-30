#pragma once
#include <vector>
#include <algorithm>
#include<math.h>

# define M_PI           3.14159265358979323846  /* pi */

/// @brief Represents a separable kernel
/// @tparam T element type
/// @tparam N size
template <typename T, unsigned int N>
class Kernel
{
public:
    Kernel(const T horizontal[N], const T vertical[N]) : _horizontal(horizontal, horizontal + N), _vertical(vertical, vertical + N) {}

    Kernel() : _horizontal(N), _vertical(N) {}

    const std::vector<T> &GetHorizontal() { return _horizontal; }

    const std::vector<T> &GetGetVertical() { return _vertical; }

    int GetSize() { return N; }

protected:
    std::vector<T> _horizontal;
    std::vector<T> _vertical;
};

/// @brief Represents a gaussian kernel
template <typename T, unsigned int N>
class Gaussian : public Kernel<T, N>
{
public:
    Gaussian(T sigma) : Kernel<T, N>()
    {
        T sigmaSqr = sigma * sigma;
        T div = T(1) / std::sqrt(2 * M_PI * sigmaSqr);
        int size = N;
        int halfsize = size / 2;

        T sum = 0;
        for (int x = -halfsize; x <= halfsize; x++)
        {
            T val = std::exp(-0.5f * x * x / sigmaSqr) * div;
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