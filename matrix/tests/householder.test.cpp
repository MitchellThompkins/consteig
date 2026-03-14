#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(householder, eigen_comparison)
{
    static constexpr Size s{4};
    static constexpr Matrix<double, s, s> mat = {
        {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}}};

    // Consteig Householder (Calculate at compile time)
    static constexpr Matrix<double, s, s> houseMat = house(mat);

    // Check properties using Eigen (Runtime)
    Eigen::MatrixXd eigHouse = toEigen(houseMat);
    Eigen::MatrixXd eigMat = toEigen(mat);

    // 1. Is unitary?
    EXPECT_TRUE(eigHouse.isUnitary(CONSTEIG_TEST_TOLERANCE));

    // 2. Does it zero out elements below a(1,0)? (indices 2, 3...)
    // house() is designed for Hessenberg reduction, preserving row 0 and
    // creating zeros in col 0 below row 1.
    Eigen::MatrixXd result = eigHouse * eigMat;

    for (Size i = 2; i < s; ++i)
    {
        EXPECT_NEAR(result(i, 0), 0.0, CONSTEIG_TEST_TOLERANCE);
    }

    // 3. Does it leave row 0 affected?
    // Actually P = diag(1, P') where P' acts on subvector.
    // So (P*A).row(0) == A.row(0) if P is block diag(1, ...).
    // Let's check P structure.
    EXPECT_NEAR(houseMat(0, 0), 1.0, CONSTEIG_TEST_TOLERANCE);
    for (Size i = 1; i < s; ++i)
    {
        EXPECT_NEAR(houseMat(0, i), 0.0, CONSTEIG_TEST_TOLERANCE);
        EXPECT_NEAR(houseMat(i, 0), 0.0, CONSTEIG_TEST_TOLERANCE);
    }
}

TEST(householder, house)
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

    static constexpr Matrix<double, s, s> test{house(mat)};

    static constexpr Matrix<double, s, s> answer{{{
        {1.000000000, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -0.181172707, 0.433798583, 0.144738036, 0.692313876, 0.253079561,
         0.198077750, 0.231951437, 0.056710547, 0.344159879},
        {0, 0.433798583, 0.840682730, -0.053156625, -0.254259836, -0.092946234,
         -0.072746218, -0.085186700, -0.020827568, -0.126396476},
        {0, 0.144738036, -0.053156625, 0.982264152, -0.084834462, -0.031011755,
         -0.024271967, -0.028422766, -0.006949173, -0.042172516},
        {0, 0.692313876, -0.254259836, -0.084834462, 0.594218102, -0.148336048,
         -0.116098158, -0.135952344, -0.033239422, -0.201720425},
        {0, 0.253079561, -0.092946234, -0.031011755, -0.148336048, 0.945774853,
         -0.042440390, -0.049698209, -0.012150874, -0.073740132},
        {0, 0.198077750, -0.072746218, -0.024271967, -0.116098158, -0.042440390,
         0.966783185, -0.038897291, -0.009510123, -0.057714180},
        {0, 0.231951437, -0.085186700, -0.028422766, -0.135952344, -0.049698209,
         -0.038897291, 0.954450802, -0.011136469, -0.067584002},
        {0, 0.056710547, -0.020827568, -0.006949173, -0.033239422, -0.012150874,
         -0.009510123, -0.011136469, 0.997277209, -0.016523828},
        {0, 0.344159879, -0.126396476, -0.042172516, -0.201720425, -0.073740132,
         -0.057714180, -0.067584002, -0.016523828, 0.899721674},
    }}};

    static_assert(compareFloatMat(test, answer, CONSTEIG_TEST_TOLERANCE), MSG);
    ASSERT_TRUE(compareFloatMat(test, answer, CONSTEIG_TEST_TOLERANCE));
}

TEST(householder, house_single)
{
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{
        {-2.0114, -0.52132},
        {-0.42729, -0.47479},
    }}};

    static constexpr Matrix<float, s, s> test{house(mat)};

    static constexpr Matrix<float, s, s> answer{{{
        {1.0, 0.0},
        {0.0, -1.0},
    }}};

    static_assert(compareFloatMat(test, answer, CONSTEIG_FLOAT_TEST_TOLERANCE),
                  MSG);
    ASSERT_TRUE(compareFloatMat(test, answer, CONSTEIG_FLOAT_TEST_TOLERANCE));
}

TEST(householder, properties)
{
    static constexpr int s{4};
    static constexpr Matrix<double, s, s> mat = {
        {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}}};

    static constexpr Matrix<double, s, s> P = house(mat);

    // Symmetric: P = P^T
    static constexpr Matrix<double, s, s> PT = transpose(P);

    static_assert(compareFloatMat(P, PT, CONSTEIG_TEST_TOLERANCE), MSG);

    for (size_t i = 0; i < s; ++i)
    {
        for (size_t j = 0; j < s; ++j)
        {
            ASSERT_NEAR(P(i, j), PT(i, j), CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Orthogonal: P^T * P = I  => P * P = I (since symmetric)
    static constexpr Matrix<double, s, s> P2 = P * P;
    static constexpr Matrix<double, s, s> I = eye<double, s>();

    static_assert(compareFloatMat(P2, I, CONSTEIG_TEST_TOLERANCE), MSG);

    for (size_t i = 0; i < s; ++i)
    {
        for (size_t j = 0; j < s; ++j)
        {
            ASSERT_NEAR(P2(i, j), I(i, j), CONSTEIG_TEST_TOLERANCE);
        }
    }
}
