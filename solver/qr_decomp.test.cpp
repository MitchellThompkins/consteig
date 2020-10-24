#include <gtest/gtest.h>
#include "qr_decomp.hpp"

TEST(qr_decomp, static)
{
    static constexpr int x {3};

    static constexpr Matrix<float, x, x> mat
    {{{{1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 0.0F, 1.0F}}}};

    //static constexpr QrMatrix<float, x> qr {qrDecomp(mat)};

    //ASSERT_NEAR(qr, mat, 0.0001F);
}
