#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"
#include "test_tools.hpp"

using namespace consteig;

static constexpr double kTol = 1e-9;

TEST(qr_decomp, eigen_comparison) {
    static constexpr Size s{4};
    static constexpr Matrix<double, s, s> mat = {{{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}}};

    // Consteig QR (Calculate at compile time)
    static constexpr QRMatrix<double, s> qrRes = qr(mat);

    // Verify reconstruction and orthogonality at compile time
    static constexpr Matrix<double, s, s> recon = qrRes._q * qrRes._r;
    static_assert(compareFloatMat(recon, mat, kTol), MSG);

    static constexpr Matrix<double, s, s> qtq = transpose(qrRes._q) * qrRes._q;
    static constexpr Matrix<double, s, s> ident = eye<double, s>();
    static_assert(compareFloatMat(qtq, ident, kTol), MSG);

    // Eigen QR (Calculate at runtime for comparison)
    Eigen::MatrixXd eigMat = toEigen(mat);
    Eigen::HouseholderQR<Eigen::MatrixXd> qrEig(eigMat);
    Eigen::MatrixXd qEig = qrEig.householderQ();
    Eigen::MatrixXd rEig = qrEig.matrixQR().triangularView<Eigen::Upper>();

    // Runtime checks
    for (size_t i = 0; i < s; ++i) {
        for (size_t j = 0; j < s; ++j) {
            ASSERT_NEAR(recon(i, j), mat(i, j), kTol);
        }
    }

    for (size_t i = 0; i < s; ++i) {
        for (size_t j = 0; j < s; ++j) {
            ASSERT_NEAR(qtq(i, j), ident(i, j), kTol);
        }
    }

    // Compare R diagonal absolute values (since signs can flip)
    for (Size i = 0; i < s; ++i) {
        ASSERT_NEAR(std::abs(qrRes._r(i, i)), std::abs(rEig(i, i)), kTol);
    }
}

TEST(qr_decomp, identity_matrix) {
    static constexpr Size s{3};
    static constexpr Matrix<double, s, s> mat = eye<double, s>();
    static constexpr QRMatrix<double, s> qrRes = qr(mat);

    // Checks
    static_assert(compareFloatMat(qrRes._q, mat, kTol), MSG);
    static_assert(compareFloatMat(qrRes._r, mat, kTol), MSG);

    for (size_t i = 0; i < s; ++i) {
        for (size_t j = 0; j < s; ++j) {
            ASSERT_NEAR(qrRes._q(i, j), mat(i, j), kTol);
            ASSERT_NEAR(qrRes._r(i, j), mat(i, j), kTol);
        }
    }
}

TEST(qr_decomp, zero_matrix) {
    static constexpr Size s{3};
    static constexpr Matrix<double, s, s> mat{};
    static constexpr QRMatrix<double, s> qrRes = qr(mat);

    static constexpr Matrix<double, s, s> ident = eye<double, s>();
    static constexpr Matrix<double, s, s> zeroMat{};

    static_assert(compareFloatMat(qrRes._q, ident, kTol), MSG);
    static_assert(compareFloatMat(qrRes._r, zeroMat, kTol), MSG);

    for (size_t i = 0; i < s; ++i) {
        for (size_t j = 0; j < s; ++j) {
            ASSERT_NEAR(qrRes._q(i, j), ident(i, j), kTol);
            ASSERT_NEAR(qrRes._r(i, j), 0.0, kTol);
        }
    }
}

TEST(qr_decomp, diagonal_matrix) {
    static constexpr Size s{3};
    static constexpr Matrix<double, s, s> mat = {{{{2, 0, 0}, {0, 3, 0}, {0, 0, 4}}}};
    static constexpr QRMatrix<double, s> qrRes = qr(mat);

    static constexpr Matrix<double, s, s> ident = eye<double, s>();

    static_assert(compareFloatMat(qrRes._q, ident, kTol), MSG);
    static_assert(compareFloatMat(qrRes._r, mat, kTol), MSG);

    for (size_t i = 0; i < s; ++i) {
        for (size_t j = 0; j < s; ++j) {
            ASSERT_NEAR(qrRes._q(i, j), (i == j ? 1.0 : 0.0), kTol);
            ASSERT_NEAR(qrRes._r(i, j), mat(i, j), kTol);
        }
    }
}

TEST(qr_decomp, singular_matrix) {
    static constexpr Size s{3};
    // Row 3 is sum of Row 1 and Row 2 -> Singular
    static constexpr Matrix<double, s, s> mat = {{{{1, 2, 3}, {4, 5, 6}, {5, 7, 9}}}};
    static constexpr QRMatrix<double, s> qrRes = qr(mat);

    static constexpr Matrix<double, s, s> recon = qrRes._q * qrRes._r;
    static_assert(compareFloatMat(recon, mat, kTol), MSG);

    for (size_t i = 0; i < s; ++i) {
        for (size_t j = 0; j < s; ++j) {
            ASSERT_NEAR(recon(i, j), mat(i, j), kTol);
        }
    }
}

TEST(qr_decomp, static_constexpr_even_mat) {
    static constexpr Size x{3};

    static constexpr Matrix<double, x, x> mat{{{{1.0, 1.0, 0.0}, {1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}}}};

    static constexpr QRMatrix<double, x> test{qr(mat)};

    // Test Static Assertion
    static_assert(compareFloatMat(test._q * test._r, mat, kTol), MSG);

    // Runtime checks
    ASSERT_TRUE(compareFloatMat(test._q * test._r, mat, kTol));
}

TEST(qr_decomp, static_constexpr_random) {
    static constexpr int s{10};
    static constexpr Matrix<double, s, s> mat{{{
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

    static constexpr auto test{qr(mat)};

    static constexpr Matrix<double, s, s> qrCheck{test._q * test._r};

    // Verify properties
    static_assert(compareFloatMat(qrCheck, mat, kTol), MSG);
    ASSERT_TRUE(compareFloatMat(qrCheck, mat, kTol));

    // Check Q unitary
    static constexpr Matrix<double, s, s> qUnitary{test._q * transpose(test._q)};
    static constexpr Matrix<double, s, s> identity{eye<double, s>()};
    static_assert(compareFloatMat(qUnitary, identity, kTol), MSG);
    ASSERT_TRUE(compareFloatMat(qUnitary, identity, kTol));
}
