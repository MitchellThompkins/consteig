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
    static constexpr Matrix<float, s, s> mat{{{
        {-2.0114F, -0.52132F, -0.28604F, 2.2908F, -0.52351F, 2.4257F, -0.59398F,
         0.027539F, 0.2731F, 0.60314F},
        {-0.42729F, -0.47479F, -0.28187F, -0.6335F, -0.84281F, -0.88644F,
         -0.77489F, -0.36081F, 0.76563F, 0.28955F},
        {1.0231F, 0.76388F, 0.37392F, -1.5837F, 0.22278F, -1.9364F, 0.54418F,
         0.19389F, -0.6478F, 1.1075F},
        {0.34136F, 1.764F, -1.1581F, 1.0057F, 0.312F, 0.87686F, 0.31207F,
         -0.45453F, -1.3822F, -0.72451F},
        {1.6328F, -1.7138F, -0.4628F, 0.76678F, -0.85746F, -1.0017F, -1.0454F,
         0.31179F, -0.71718F, -0.19216F},
        {0.59688F, -0.4885F, 1.3867F, 1.8407F, 0.076099F, 0.33187F, -0.86447F,
         -0.2873F, -0.50884F, -1.1118F},
        {0.46716F, -0.62785F, 0.6553F, 0.8872F, -0.9114F, 0.91303F, 0.45458F,
         -0.43831F, -0.45411F, -1.1096F},
        {0.54705F, 1.7937F, -0.7893F, -0.11453F, -1.3022F, -0.4695F, -0.48678F,
         -0.23694F, -0.4487F, 0.35993F},
        {0.13375F, 1.4146F, 0.03702F, 0.29992F, -0.27342F, -1.1086F, 0.6296F,
         -1.4377F, 1.4777F, -3.0378F},
        {0.81169F, 0.3244F, 2.2287F, -0.72454F, -1.6842F, -1.5909F, -1.0693F,
         0.72293F, -1.4584F, 0.68517F},
    }}};

    static constexpr PHMatrix<float, s> test{hess(mat)};

    static constexpr Matrix<float, s, s> pAnswer{{{
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -0.18117F, 0.44375F, -0.22824F, -0.34168F, 0.25395F, -0.37373F,
         0.013938F, -0.41826F, 0.4713F},
        {0, 0.43379F, -0.17018F, 0.57061F, 0.034856F, 0.23521F, -0.17301F,
         0.5226F, -0.044992F, 0.30928F},
        {0, 0.14474F, 0.15479F, -0.20855F, 0.67035F, 0.14485F, 0.42362F,
         0.078914F, -0.4927F, 0.11317F},
        {0, 0.69232F, 0.011963F, -0.078918F, -0.011409F, -0.20284F, -0.39861F,
         -0.40094F, -0.29951F, -0.25236F},
        {0, 0.25308F, -0.2662F, -0.24113F, -0.18194F, -0.43341F, 0.30468F,
         -0.11895F, 0.11739F, 0.68208F},
        {0, 0.19807F, 0.01921F, -0.6511F, 0.19194F, 0.063358F, -0.31624F,
         0.47284F, 0.41151F, -0.051815F},
        {0, 0.23195F, 0.52535F, 0.16779F, 0.18116F, 0.29864F, 0.082897F,
         -0.40283F, 0.55235F, 0.21403F},
        {0, 0.056709F, 0.62881F, 0.17086F, -0.070579F, -0.63279F, 0.11747F,
         0.37137F, -0.0078353F, -0.12251F},
        {0, 0.34416F, 0.085951F, -0.18211F, -0.56991F, 0.36958F, 0.52777F,
         0.14764F, -0.047875F, -0.27747F},
    }}};

    static constexpr Matrix<float, s, s> hAnswer{{{
        {-2.0114F, 0.66518F, -0.25342F, -0.73737F, 0.79638F, -0.7923F, 2.703F,
         -0.15611F, -0.71645F, 1.5476F},
        {2.3585F, -1.2051F, -0.75753F, 1.228F, 0.83358F, 2.3645F, 0.14301F,
         -0.033031F, -0.20286F, -0.21725F},
        {0, -2.8129F, 1.0663F, -0.21271F, -0.039781F, -0.53001F, -1.1983F,
         0.74121F, -0.94743F, 0.26473F},
        {0, 0, 1.5046F, -0.4789F, -1.9881F, 0.52051F, -1.5516F, -0.060124F,
         0.24774F, -1.5746F},
        {0, 0, 0, -3.1888F, 0.34022F, -1.0192F, -0.41728F, -1.1957F, -0.86881F,
         1.1545F},
        {0, 0, 0, 0, -2.7698F, 2.5429F, 0.83127F, -0.4262F, 1.13F, -0.44189F},
        {0, 0, 0, 0, 0, 1.2561F, -0.96371F, -0.0031668F, -1.1761F, 1.7833F},
        {0, 0, 0, 0, 0, 0, -2.0585F, 1.2918F, 0.13721F, 0.21413F},
        {0, 0, 0, 0, 0, 0, 0, 0.77144F, 0.017001F, 0.96154F},
        {0, 0, 0, 0, 0, 0, 0, 0, -0.94249F, 0.14928F},
    }}};

    // P*H*P' = A where A is the input matrix
    static constexpr Matrix<float, s, s> hessCheck{test._p * test._h *
                                                   transpose(test._p)};

    // P*P' = Identity matrix
    static constexpr Matrix<float, s, s> identity{eye<float, s>()};
    static constexpr Matrix<float, s, s> identityCheck{test._p *
                                                       transpose(test._p)};

    static_assert(
        approxEqualMat(test._p, pAnswer, CONSTEIG_ITERATIVE_FLOAT_TOLERANCE),
        MSG);
    ASSERT_TRUE(
        approxEqualMat(test._p, pAnswer, CONSTEIG_ITERATIVE_FLOAT_TOLERANCE));

    static_assert(
        approxEqualMat(test._h, hAnswer, CONSTEIG_ITERATIVE_FLOAT_TOLERANCE),
        MSG);
    ASSERT_TRUE(
        approxEqualMat(test._h, hAnswer, CONSTEIG_ITERATIVE_FLOAT_TOLERANCE));

    static_assert(
        approxEqualMat(hessCheck, mat, CONSTEIG_ITERATIVE_FLOAT_TOLERANCE),
        MSG);
    ASSERT_TRUE(
        approxEqualMat(hessCheck, mat, CONSTEIG_ITERATIVE_FLOAT_TOLERANCE));

    static_assert(approxEqualMat(identity, identityCheck,
                                 CONSTEIG_ITERATIVE_FLOAT_TOLERANCE),
                  MSG);
    ASSERT_TRUE(approxEqualMat(identity, identityCheck,
                               CONSTEIG_ITERATIVE_FLOAT_TOLERANCE));
}

TEST(hessenberg, hess_double_10x10)
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
    static_assert(approxEqualMat(hessCheck, mat, CONSTEIG_TEST_TOLERANCE), MSG);
    static_assert(
        approxEqualMat(identity, identityCheck, CONSTEIG_TEST_TOLERANCE), MSG);

    ASSERT_TRUE(approxEqualMat(hessCheck, mat, CONSTEIG_TEST_TOLERANCE));
    ASSERT_TRUE(
        approxEqualMat(identity, identityCheck, CONSTEIG_TEST_TOLERANCE));

    static_assert(approxEqualMat(test._h, hAnswer, CONSTEIG_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(approxEqualMat(test._h, hAnswer, CONSTEIG_TEST_TOLERANCE));
}
