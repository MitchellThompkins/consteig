#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "qr_decomp.hpp"
#include "stdint.h"
#include "gcem.hpp"

static constexpr float kThresh {0.0001F};

template<typename T, size_t R, size_t C>
static inline constexpr bool compare(Matrix<T,R,C> a, Matrix<T,R,C> b)
{
    for(int i {0}; i<R; i++)
    {
        for(int j {0}; j<C; j++)
        {
            if( !compareFloat(a(i,j), b(i,j), kThresh) )
                return false;
        }
    }
    return true;
}

template<typename T, size_t R, size_t C>
static inline constexpr bool checkEigenValues(
        const Matrix<T,R,C> &a,
        const Matrix<T,R,1> lambda )
{
    //det(A-lambda*I)
    bool equal {true};
    auto identity {diagional<T,R>(static_cast<T>(1))};

    for(int i {0}; i<R; i++)
    {
        equal &= compareFloat( det( a - lambda(i,0)*identity), static_cast<T>(0), kThresh );
    }

    return equal;
}

TEST(qr_decomp, static_constexpr_even_mat)
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
    static_assert(compare(qr._q, qAnswer), MSG);
    static_assert(compare(qr._r, rAnswer), MSG);

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

}

TEST(qr_decomp, static_constexpr_random)
{
    static constexpr size_t x {3};

    static constexpr Matrix<float, x, x> mat
    {{{ {1.0F, 6.0F, 62.0F}, {1.0F, 5.0F , 22.0F}, {-1.0F, 3.0F, 11.0F} }}};

    static constexpr QrMatrix<float, x> qr {qrDecomp(mat)};

    static constexpr Matrix<float, x, x> qAnswer
    {{{
        {  0.5773502691896258F, 0.4778184825674966F,  0.6620847108818944F  },
        {  0.5773502691896258F, 0.3344729377972476F, -0.744845299742131F   },
        { -0.5773502691896258F, 0.8122914203647441F, -0.08276058886023681F }
    }}};

    static constexpr Matrix<float, x, x> rAnswer
    {{{
        {1.7320508075688772F,  4.618802153517007F, 42.146569650842686F },
        {0.0F,                 6.97614984548545F,  45.91835617473642F  },
        {0.0F,                 0.0F,               23.752289002887967F }
    }}};

    // Test Static Assertion
    static_assert(compare(qr._q, qAnswer), MSG);
    static_assert(compare(qr._r, rAnswer), MSG);

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

}

TEST(qr_decomp, static_constexpr_eigenValues)
{
    static constexpr size_t x {3};

    static constexpr Matrix<float, x, x> mat
    {{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 1.0F, 1.0F} }}};

    static constexpr auto eigenValueTest {eigenvalues(mat)};

    //static_assert(checkEigenValues<float,x,x>(mat,eigenValueTest), MSG);
    //ASSERT_TRUE( checkEigenValues<float,x,x>(mat,eigenValueTest) );
}
