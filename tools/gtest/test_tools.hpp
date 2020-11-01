#ifndef TEST_TOOLS_HPP
#define TEST_TOOLS_HPP

#include "gcem.hpp"

#define MSG "Not constexpr or wrong output"

template<typename T>
static inline constexpr bool compareFloat(T a, T b, T thresh)
{
    return gcem::abs(a - b) < thresh;
}

#endif
