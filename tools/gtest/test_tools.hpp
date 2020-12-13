#ifndef TEST_TOOLS_HPP
#define TEST_TOOLS_HPP

#include "gcem.hpp"
#include "../consteig.hpp"

#define MSG "Not constexpr or wrong output"

static constexpr bool compareFloat(float a, float b, float thresh)
{
    return gcem::abs(a - b) < thresh;
}

template<size_t R, size_t C>
static constexpr bool compareFloatMat(
        consteig::Matrix<float,R,C> a,
        consteig::Matrix<float,R,C> b,
        const float thresh )
{
    for(int i {0}; i<R; i++)
    {
        for(int j {0}; j<C; j++)
        {
            if( !compareFloat(a(i,j), b(i,j), thresh) )
                return false;
        }
    }
    return true;
}

#endif
