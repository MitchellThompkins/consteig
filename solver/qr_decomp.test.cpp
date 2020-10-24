#include <gtest/gtest.h>
#include "qr_decomp.hpp"
#include "gcem.hpp"

TEST(qr_decomp, static)
{
    static constexpr int x {3};

    static constexpr Matrix<float, x, x> mat
    {{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 0.0F, 1.0F} }}};

    static constexpr QrMatrix<float, x> qr {qrDecomp(mat)};

    static constexpr Matrix<float, x, x> qAnswer
    {{{
        {1.0F/gcem::sqrt(2),  1.0F/gcem::sqrt(6), -1.0F/gcem::sqrt(3)},
        {1.0F/gcem::sqrt(2), -1.0F/gcem::sqrt(6),  1.0F/gcem::sqrt(3)},
        {0.0F,                2.0F/gcem::sqrt(6),  1.0F/gcem::sqrt(3)}
    }}};

    //static_assert(qr._q == qAnswer, "Not constexpr");

    //ASSERT_NEAR(qr, mat, 0.0001F);
    ASSERT_TRUE(qr._q == qAnswer);
}
