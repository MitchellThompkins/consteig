#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "decompositions.hpp"

using namespace consteig;

static constexpr float kThresh {0.0001F};

TEST(qr_decomp, static_constexpr_even_mat)
{
    static constexpr size_t x {3};

    static constexpr Matrix<float, x, x> mat
    {{{ {1.0F, 1.0F, 0.0F}, {1.0F, 0.0F , 1.0F}, {0.0F, 1.0F, 1.0F} }}};

    static constexpr QRMatrix<float, x> test {qr(mat)};

    static constexpr Matrix<float, x, x> qAnswer
    {{{
        {1.0F/consteig::sqrt(2.0F),  1.0F/consteig::sqrt(6.0F), -1.0F/consteig::sqrt(3.0F)},
        {1.0F/consteig::sqrt(2.0F), -1.0F/consteig::sqrt(6.0F),  1.0F/consteig::sqrt(3.0F)},
        {0.0F,                       2.0F/consteig::sqrt(6.0F),  1.0F/consteig::sqrt(3.0F)}
    }}};

    static constexpr Matrix<float, x, x> rAnswer
    {{{
        {2.0F/consteig::sqrt(2.0F), 1.0F/consteig::sqrt(2.0F), 1.0F/consteig::sqrt(2.0F)},
        {0.0F,                      3.0F/consteig::sqrt(6.0F), 1.0F/consteig::sqrt(6.0F)},
        {0.0F,                      0.0F,                      2.0F/consteig::sqrt(3.0F)}
    }}};

    // Test Static Assertion
    static_assert(compareFloatMat(test._q, qAnswer, kThresh), MSG);
    static_assert(compareFloatMat(test._r, rAnswer, kThresh), MSG);

    // Technically these next two checks aren't necessary if the static asserts
    // are passed
    for(int i {0}; i<x; i++)
    {
        for(int j {0}; j<x; j++)
        {
            ASSERT_NEAR(test._q(i,j), qAnswer(i,j), .0001F);
        }
    }

    for(int i {0}; i<x; i++)
        for(int j {0}; j<x; j++)
            ASSERT_NEAR(test._r(i,j), rAnswer(i,j), .0001F);

}

TEST(qr_decomp, static_constexpr_random)
{
    static constexpr int s {10};
    static constexpr Matrix<float,s,s> mat
    {{{
    {-2.0114, -0.52132, -0.28604, 2.2908, -0.52351, 2.4257, -0.59398, 0.027539, 0.2731, 0.60314},
    {-0.42729, -0.47479, -0.28187, -0.6335, -0.84281, -0.88644, -0.77489, -0.36081, 0.76563, 0.28955},
    {1.0231, 0.76388, 0.37392, -1.5837, 0.22278, -1.9364, 0.54418, 0.19389, -0.6478, 1.1075},
    {0.34136, 1.764, -1.1581, 1.0057, 0.312, 0.87686, 0.31207, -0.45453, -1.3822, -0.72451},
    {1.6328, -1.7138, -0.4628, 0.76678, -0.85746, -1.0017, -1.0454, 0.31179, -0.71718, -0.19216},
    {0.59688, -0.4885, 1.3867, 1.8407, 0.076099, 0.33187, -0.86447, -0.2873, -0.50884, -1.1118},
    {0.46716, -0.62785, 0.6553, 0.8872, -0.9114, 0.91303, 0.45458, -0.43831, -0.45411, -1.1096},
    {0.54705, 1.7937, -0.7893, -0.11453, -1.3022, -0.4695, -0.48678, -0.23694, -0.4487, 0.35993},
    {0.13375, 1.4146, 0.03702, 0.29992, -0.27342, -1.1086, 0.6296, -1.4377, 1.4777, -3.0378},
    {0.81169, 0.3244, 2.2287, -0.72454, -1.6842, -1.5909, -1.0693, 0.72293, -1.4584, 0.68517},
    }}};

    static constexpr QRMatrix<float, s> test {qr(mat)};

    static constexpr Matrix<float, s, s> qrCheck {test._q*test._r};

    static constexpr Matrix<float, s, s> qAnswer
    {{{
    {-0.64889, -0.10463, -0.05114, -0.42058, -0.29853, -0.11557, -0.004846, 0.45357, -0.14972, -0.2405},
    {-0.13785, -0.12305, 0.088758, 0.2546, -0.44879, -0.34022, 0.12505, -0.56567, -0.4825, -0.087038},
    {0.33006, 0.19134, -0.085165, 0.29382, 0.14756, -0.1572, -0.1576, 0.37036, -0.39117, -0.63195},
    {0.11013, 0.48181, 0.30571, -0.40604, 0.12758, 0.17171, 0.16467, 0.017903, -0.58284, 0.29069},
    {0.52677, -0.50692, 0.42152, -0.24136, -0.26171, -0.25898, 0.023186, 0.28436, 0.0073938, 0.11222},
    {0.19256, -0.14709, -0.38372, -0.56955, 0.30617, -0.21765, 0.31499, -0.34626, 0.027123, -0.32969},
    {0.15071, -0.18312, -0.13771, -0.26464, -0.2321, 0.52631, -0.64319, -0.24602, -0.16852, -0.14581},
    {0.17649, 0.48596, 0.1972, -0.11945, -0.52866, 0.14388, 0.22751, -0.091654, 0.42311, -0.37439},
    {0.043148, 0.38913, -0.095644, -0.19092, -0.047927, -0.63436, -0.56648, -0.070248, 0.15916, 0.21267},
    {0.26186, 0.073804, -0.70489, 0.060881, -0.41657, 0.046127, 0.21512, 0.24938, -0.13606, 0.35039},
    }}};

    static constexpr Matrix<float, s, s> rAnswer
    {{{
    {3.0997, 0.22117, 0.78826, -1.116, -0.69328, -2.8678, -0.28116, 0.17412, -1.5903, -0.51622},
    {0, 3.6107, -0.71586, -0.79456, 0.078267, -0.57793, 0.91535, -0.79714, -0.14183, -0.72842},
    {0, 0, -2.9439, 0.2231, 0.71981, 0.68974, 0.43667, -0.30568, 0.44025, 0.056035},
    {0, 0, 0, -3.5543, 0.45439, -2.1888, 0.58292, 0.68926, 0.71674, 1.9916},
    {0, 0, 0, 0, 2.4696, 0.6157, 1.2212, -0.050486, 0.21412, -0.6017},
    {0, 0, 0, 0, 0, 1.7063, 0.4797, 0.6734, -1.4393, 1.2515},
    {0, 0, 0, 0, 0, 0, -1.4147, 0.96402, -1.1688, 2.0486},
    {0, 0, 0, 0, 0, 0, 0, 0.87923, -0.91627, 1.4616},
    {0, 0, 0, 0, 0, 0, 0, 0, 0.9499, -0.50995},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, -1.1147},
    }}};

    //TODO(mthompkins): Readdress this
    //static_assert(compareFloatMat(test._q, qAnswer, kThresh), MSG);
    //ASSERT_TRUE(compareFloatMat(test._q, qAnswer, kThresh));

    //static_assert(compareFloatMat(test._r, rAnswer, kThresh), MSG);
    //ASSERT_TRUE(compareFloatMat(test._r, rAnswer, kThresh));

    static_assert(compareFloatMat(qrCheck, mat, kThresh), MSG);
    ASSERT_TRUE(compareFloatMat(qrCheck, mat, kThresh));
}
