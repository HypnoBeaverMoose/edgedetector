#include <iostream>
#include <vector>
#include "kernels.hpp"
#include "image.hpp"
#include "image-factory.hpp"

int main(int argc, char **argv)
{
    Image im = PredefinedImageFactory().GetImage();
    im.Convolve(3, identity);
    std::cout<<im;
}
