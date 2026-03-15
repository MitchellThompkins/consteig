#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"

using namespace consteig;

TEST(hessenberg, eigen_comparison)
{
    static constexpr Size s{4};
    static constexpr Matrix<double, s, s> mat = {
        {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}}};

    // Consteig Hessenberg (Calculate at compile time)
    static constexpr PHMatrix<double, s> hessRes = hess(mat);

    // Eigen Hessenberg (Runtime)
    Eigen::MatrixXd eigMat = toEigen(mat);
    Eigen::HessenbergDecomposition<Eigen::MatrixXd> hessEig(eigMat);
    Eigen::MatrixXd hEig = hessEig.matrixH();

    // Check if H is Upper Hessenberg
    for (Size i = 2; i < s; ++i)
    {
        for (Size j = 0; j < i - 1; ++j)
        {
            EXPECT_NEAR(hessRes._h(i, j), 0.0, CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Check eigenvalues of H match eigenvalues of A (using Eigen to compute
    // eigenvalues of H)
    Eigen::MatrixXd hConstEig = toEigen(hessRes._h);
    Eigen::VectorXcd eigValsH = hConstEig.eigenvalues();
    Eigen::VectorXcd eigValsA = eigMat.eigenvalues();

    // Compare H elements roughly (signs might differ)
    for (Size i = 0; i < s; ++i)
    {
        for (Size j = 0; j < s; ++j)
        {
            EXPECT_NEAR(std::abs(hessRes._h(i, j)),
                        std::abs(hEig(static_cast<Eigen::Index>(i),
                                      static_cast<Eigen::Index>(j))),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }
}

TEST(hessenberg, hess)
{
    static constexpr Size s{10};
    static constexpr Matrix<double, s, s> mat{{{
        {-2.0114, -0.52132, -0.28604, 2.2908, -0.52351, 2.4257, -0.59398,
         0.027539, 0.2731, 0.60314},
        {-0.42729, -0.47479, -0.28187, -0.6335, -0.84281, -0.88644, -0.77489,
         -0.36081, 0.76563, 0.28955},
        {1.0231, 0.76388, 0.37392, -1.5837, 0.22278, -1.9364, 0.54418, 0.19389,
         -0.6478, 1.1075},
        {0.34136, 1.764, -1.1581, 1.0057, 0.312, 0.87686, 0.31207, -0.45453,
         -1.3822, -0.72451},
        {1.6328, -1.7138, -0.4628, 0.76678, -0.85746, -1.0017, -1.0454, 0.31179,
         -0.71718, -0.19216},
        {0.59688, -0.4885, 1.3867, 1.8407, 0.076099, 0.33187, -0.86447, -0.2873,
         -0.50884, -1.1118},
        {0.46716, -0.62785, 0.6553, 0.8872, -0.9114, 0.91303, 0.45458, -0.43831,
         -0.45411, -1.1096},
        {0.54705, 1.7937, -0.7893, -0.11453, -1.3022, -0.4695, -0.48678,
         -0.23694, -0.4487, 0.35993},
        {0.13375, 1.4146, 0.03702, 0.29992, -0.27342, -1.1086, 0.6296, -1.4377,
         1.4777, -3.0378},
        {0.81169, 0.3244, 2.2287, -0.72454, -1.6842, -1.5909, -1.0693, 0.72293,
         -1.4584, 0.68517},
    }}};

    static constexpr PHMatrix<double, s> test{hess(mat)};

    static constexpr Matrix<double, s, s> pAnswer{{{
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -0.18117, 0.44375, -0.22824, -0.34168, 0.25395, -0.37373, 0.013938,
         -0.41826, 0.4713},
        {0, 0.43379, -0.17018, 0.57061, 0.034856, 0.23521, -0.17301, 0.5226,
         -0.044992, 0.30928},
        {0, 0.14474, 0.15479, -0.20855, 0.67035, 0.14485, 0.42362, 0.078914,
         -0.4927, 0.11317},
        {0, 0.69232, 0.011963, -0.078918, -0.011409, -0.20284, -0.39861,
         -0.40094, -0.29951, -0.25236},
        {0, 0.25308, -0.2662, -0.24113, -0.18194, -0.43341, 0.30468, -0.11895,
         0.11739, 0.68208},
        {0, 0.19807, 0.01921, -0.6511, 0.19194, 0.063358, -0.31624, 0.47284,
         0.41151, -0.051815},
        {0, 0.23195, 0.52535, 0.16779, 0.18116, 0.29864, 0.082897, -0.40283,
         0.55235, 0.21403},
        {0, 0.056709, 0.62881, 0.17086, -0.070579, -0.63279, 0.11747, 0.37137,
         -0.0078353, -0.12251},
        {0, 0.34416, 0.085951, -0.18211, -0.56991, 0.36958, 0.52777, 0.14764,
         -0.047875, -0.27747},
    }}};

    static constexpr Matrix<double, s, s> hAnswer{{{
        {-2.0114, 0.66518, -0.25342, -0.73737, 0.79638, -0.7923, 2.703,
         -0.15611, -0.71645, 1.5476},
        {2.3585, -1.2051, -0.75753, 1.228, 0.83358, 2.3645, 0.14301, -0.033031,
         -0.20286, -0.21725},
        {0, -2.8129, 1.0663, -0.21271, -0.039781, -0.53001, -1.1983, 0.74121,
         -0.94743, 0.26473},
        {0, 0, 1.5046, -0.4789, -1.9881, 0.52051, -1.5516, -0.060124, 0.24774,
         -1.5746},
        {0, 0, 0, -3.1888, 0.34022, -1.0192, -0.41728, -1.1957, -0.86881,
         1.1545},
        {0, 0, 0, 0, -2.7698, 2.5429, 0.83127, -0.4262, 1.13, -0.44189},
        {0, 0, 0, 0, 0, 1.2561, -0.96371, -0.0031668, -1.1761, 1.7833},
        {0, 0, 0, 0, 0, 0, -2.0585, 1.2918, 0.13721, 0.21413},
        {0, 0, 0, 0, 0, 0, 0, 0.77144, 0.017001, 0.96154},
        {0, 0, 0, 0, 0, 0, 0, 0, -0.94249, 0.14928},
    }}};

    // P*H*P' = A where A is the input matrix
    static constexpr Matrix<double, s, s> hessCheck{test._p * test._h *
                                                    transpose(test._p)};

    // P*P' = Identity matrix
    static constexpr Matrix<double, s, s> identity{eye<double, s>()};
    static constexpr Matrix<double, s, s> identityCheck{test._p *
                                                        transpose(test._p)};

    static_assert(
        compareFloatMat(test._p, pAnswer, CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE),
        MSG);
    ASSERT_TRUE(
        compareFloatMat(test._p, pAnswer, CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE));

    static_assert(
        compareFloatMat(test._h, hAnswer, CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE),
        MSG);
    ASSERT_TRUE(
        compareFloatMat(test._h, hAnswer, CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE));

    static_assert(
        compareFloatMat(hessCheck, mat, CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE),
        MSG);
    ASSERT_TRUE(
        compareFloatMat(hessCheck, mat, CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE));

    static_assert(compareFloatMat(identity, identityCheck,
                                  CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE),
                  MSG);
    ASSERT_TRUE(compareFloatMat(identity, identityCheck,
                                CONSTEIG_ITERATIVE_DOUBLE_TOLERANCE));
}

TEST(hessenberg, hess_double_10x10)
{
    static constexpr int s{10};
    static constexpr Matrix<double, s, s> mat{{{
        {-2.0114, -0.52132, -0.28604, 2.2908, -0.52351, 2.4257, -0.59398,
         0.027539, 0.2731, 0.60314},
        {-0.42729, -0.47479, -0.28187, -0.6335, -0.84281, -0.88644, -0.77489,
         -0.36081, 0.76563, 0.28955},
        {1.0231, 0.76388, 0.37392, -1.5837, 0.22278, -1.9364, 0.54418, 0.19389,
         -0.6478, 1.1075},
        {0.34136, 1.764, -1.1581, 1.0057, 0.312, 0.87686, 0.31207, -0.45453,
         -1.3822, -0.72451},
        {1.6328, -1.7138, -0.4628, 0.76678, -0.85746, -1.0017, -1.0454, 0.31179,
         -0.71718, -0.19216},
        {0.59688, -0.4885, 1.3867, 1.8407, 0.076099, 0.33187, -0.86447, -0.2873,
         -0.50884, -1.1118},
        {0.46716, -0.62785, 0.6553, 0.8872, -0.9114, 0.91303, 0.45458, -0.43831,
         -0.45411, -1.1096},
        {0.54705, 1.7937, -0.7893, -0.11453, -1.3022, -0.4695, -0.48678,
         -0.23694, -0.4487, 0.35993},
        {0.13375, 1.4146, 0.03702, 0.29992, -0.27342, -1.1086, 0.6296, -1.4377,
         1.4777, -3.0378},
        {0.81169, 0.3244, 2.2287, -0.72454, -1.6842, -1.5909, -1.0693, 0.72293,
         -1.4584, 0.68517},
    }}};

    static constexpr Matrix<double, s, s> hAnswer{{{
        {-2.011400000, 0.665190682, -0.253405342, -0.737285848, 0.796410871,
         -0.792480076, 2.702990155, -0.156257659, -0.716340054, 1.547604533},
        {2.358467821, -1.205092504, -0.757545400, 1.227954465, 0.833655497,
         2.364541973, 0.143043074, -0.033030817, -0.202888329, -0.217229851},
        {0, -2.812863113, 1.066275038, -0.212704275, -0.039780865, -0.530047398,
         -1.198301979, 0.741222572, -0.947515311, 0.264790391},
        {0, 0, 1.504578493, -0.479100987, -1.988043671, 0.520594600,
         -1.551621671, -0.059976648, 0.247582191, -1.574502039},
        {0, 0, 0, -3.188728666, 0.340172221, -1.019228874, -0.417172337,
         -1.195767566, -0.868722823, 1.154522534},
        {0, 0, 0, 0, -2.769667369, 2.543073845, 0.831309021, -0.426091992,
         1.130048622, -0.442093923},
        {0, 0, 0, 0, 0, 1.256132812, -0.963659108, -0.003244195, -1.176083845,
         1.783266178},
        {0, 0, 0, 0, 0, 0, -2.058448681, 1.291875568, 0.137167182, 0.214147063},
        {0, 0, 0, 0, 0, 0, 0, 0.771344211, 0.016916493, 0.961629657},
        {0, 0, 0, 0, 0, 0, 0, 0, -0.942471265, 0.149289433},
    }}};

    static constexpr PHMatrix<double, s> test{hess(mat)};

    // P*H*P' = A
    static constexpr Matrix<double, s, s> hessCheck{test._p * test._h *
                                                    transpose(test._p)};

    // P*P' = I
    static constexpr Matrix<double, s, s> identity{eye<double, s>()};
    static constexpr Matrix<double, s, s> identityCheck{test._p *
                                                        transpose(test._p)};

    // Prove that double precision allows the 1e-9 tolerance to pass
    static_assert(compareFloatMat(hessCheck, mat, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    static_assert(
        compareFloatMat(identity, identityCheck, CONSTEIG_TEST_TOLERANCE), MSG);

    ASSERT_TRUE(compareFloatMat(hessCheck, mat, CONSTEIG_TEST_TOLERANCE));
    ASSERT_TRUE(
        compareFloatMat(identity, identityCheck, CONSTEIG_TEST_TOLERANCE));

    static_assert(compareFloatMat(test._h, hAnswer, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(compareFloatMat(test._h, hAnswer, CONSTEIG_TEST_TOLERANCE));
}
