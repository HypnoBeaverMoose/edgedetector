#pragma once
#include <vector>
#include <algorithm>

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
        for (size_t i = 0; i < N; i++)
        {
            this->_horizontal[i] /= sum;
        }

        std::copy(this->_horizontal.begin(), this->_horizontal.end(), this->_vertical.begin());
    }
};

Kernel<float, 3> SobelX((float[3]){1, 0, -1}, (float[3]){1, 2, 1});
Kernel<float, 3> SobelY((float[3]){1, 2, 1}, (float[3]){1, 0, -1});
Gaussian<5> Gaussian5(2);