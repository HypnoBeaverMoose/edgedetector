#pragma once
#include "image.hpp"
#include <algorithm>
#include <iostream>

namespace Aux
{
    template <typename TRet, typename TLeft, typename TRight>
    TRet FindGradientDirection(TLeft left, TRight right)
    {
        TLeft deg = (std::atan2(left, right) * 57.2958);
        deg = deg / 45;

        int index = std::round(deg);
        if (index < 0)
        {
            index += 4;
        }
        else if (index == 4)
        {
            index = 0;
        }
        return index;
    }

    template <typename TRet, typename TLeft, typename TRight>
    TRet CombineGradients(TLeft left, TRight right)
    {
        return std::sqrt(left * left + right * right);
    }

    template <typename TRet, typename TLeft, typename TRight>
    TRet Difference(TLeft left, TRight right)
    {
        return std::abs(left - right);
    }
}