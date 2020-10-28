#include <gtest/gtest.h>
#include "qr_decomp.hpp"
#include "stdint.h"
#include "gcem.hpp"

#define THRESHOLD 0.0001F

template<typename T>
static inline constexpr bool compareFloat(T a, T b)
{
    return gcem::abs(a - b) < THRESHOLD;
}

template<typename T, size_t R, size_t C>
static inline constexpr bool compare(Matrix<T,R,C> a, Matrix<T,R,C> b)
{
    for(int i {0}; i<R; i++)
    {
        for(int j {0}; j<C; j++)
        {
            if( !compareFloat(a(i,j), b(i,j)) )
                return false;
        }
    }
    return true;
}

TEST(qr_decomp, static)
{
    static constexpr size_t x {3};

    static constexpr Matrix<float, x, x> mat
    {{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 1.0F, 1.0F} }}};

    static constexpr QrMatrix<float, x> qr {qrDecomp(mat)};

    static constexpr Matrix<float, x, x> qAnswer
    {{{
        {1.0F/gcem::sqrt(2.0F),  1.0F/gcem::sqrt(6.0F), -1.0F/gcem::sqrt(3.0F)},
        {1.0F/gcem::sqrt(2.0F), -1.0F/gcem::sqrt(6.0F),  1.0F/gcem::sqrt(3.0F)},
        {0.0F,                   2.0F/gcem::sqrt(6.0F),  1.0F/gcem::sqrt(3.0F)}
    }}};

    static constexpr Matrix<float, x, x> rAnswer
    {{{
        {2.0F/gcem::sqrt(2.0F),  1.0F/gcem::sqrt(2.0F),  1.0F/gcem::sqrt(2.0F)},
        {0.0F,                   3.0F/gcem::sqrt(6.0F),  1.0F/gcem::sqrt(6.0F)},
        {0.0F,                   0.0F,                   2.0F/gcem::sqrt(3.0F)}
    }}};

    // Test Static Assertion
    static_assert(compare(qr._q, qAnswer), "Not constexpr or wrong val");
    static_assert(compare(qr._r, rAnswer), "Not constexpr or wrong val");

    // Technically these next two checks aren't necessary if the static asserts
    // are passed
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            ASSERT_NEAR(qr._q(i,j), qAnswer(i,j), .0001F);
        }
    }

    for(int i {0}; i<x; i++)
        for(int j {0}; j<x; j++)
            ASSERT_NEAR(qr._r(i,j), rAnswer(i,j), .0001F);

    //ASSERT_NEAR(qr, mat, 0.0001F);
}
