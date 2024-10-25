#pragma once
#include <vector>

template <typename T, unsigned int N>
class Kernel
{
public:
    Kernel(const T horizontal[N], const T vertical[N]) : _horizontal(horizontal, horizontal + N), _vertical(vertical, vertical + N) {}

    const std::vector<T> &GetHorizontal() { return _horizontal; }

    const std::vector<T> &GetGetVertical() { return _vertical; }

    int GetSize() { return N; }

private:
    std::vector<T> _horizontal;
    std::vector<T> _vertical;
};

Kernel<float, 3> SobelX((float[3]){1, 0, -1}, (float[3]){1, 2, 1});
Kernel<float, 3> SobelY((float[3]){1, 2, 1}, (float[3]){1, 0, -1});