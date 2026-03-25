#include <gtest/gtest.h>

#include "decompositions.hpp"
#include "eigen_test_tools.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(qr_decomp, eigen_comparison)
{
    static constexpr Size s{4};
    static constexpr Matrix<double, s, s> mat = {
        {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}};

    // Consteig QR (Calculate at compile time)
    static constexpr QRMatrix<double, s> qrRes = qr(mat);

    // Verify reconstruction and orthogonality at compile time
    static constexpr Matrix<double, s, s> recon = qrRes._q * qrRes._r;
    static_assert(equalWithinMat(recon, mat, CONSTEIG_TEST_TOLERANCE), MSG);

    static constexpr Matrix<double, s, s> qtq = transpose(qrRes._q) * qrRes._q;
    static constexpr Matrix<double, s, s> ident = eye<double, s>();
    static_assert(equalWithinMat(qtq, ident, CONSTEIG_TEST_TOLERANCE), MSG);

    // Eigen QR (Calculate at runtime for comparison)
    Eigen::MatrixXd eigMat = toEigen(mat);
    Eigen::HouseholderQR<Eigen::MatrixXd> qrEig(eigMat);
    Eigen::MatrixXd qEig = qrEig.householderQ();
    Eigen::MatrixXd rEig = qrEig.matrixQR().triangularView<Eigen::Upper>();

    // Runtime checks
    for (Size i = 0; i < s; ++i)
    {
        for (Size j = 0; j < s; ++j)
        {
            ASSERT_NEAR(recon(i, j), mat(i, j), CONSTEIG_TEST_TOLERANCE);
        }
    }

    for (Size i = 0; i < s; ++i)
    {
        for (Size j = 0; j < s; ++j)
        {
            ASSERT_NEAR(qtq(i, j), ident(i, j), CONSTEIG_TEST_TOLERANCE);
        }
    }

    // Compare R diagonal absolute values (since signs can flip)
    for (Size i = 0; i < s; ++i)
    {
        ASSERT_NEAR(std::abs(qrRes._r(i, i)),
                    std::abs(rEig(static_cast<Eigen::Index>(i),
                                  static_cast<Eigen::Index>(i))),
                    CONSTEIG_TEST_TOLERANCE);
    }

    // Compare Q column-by-column using absolute values to handle sign
    // ambiguity. QR decomposition is only unique up to column sign: both
    // consteig and Eigen may negate an entire Q column (paired with the same
    // negation in the corresponding R row) and still produce a valid
    // factorization. Taking abs() of each element neutralizes that.
    //
    // Additionally, the test matrix {1..4; 5..8; 9..12; 13..16} is rank-2
    // (each row is an arithmetic progression, so rows 3 and 4 are linear
    // combinations of rows 1 and 2). This means two of the four Q columns
    // span the null space. For null-space columns, the Householder process
    // has no unique answer — any orthonormal completion of the column space
    // is valid — so consteig and Eigen will freely disagree. We detect these
    // columns by checking whether the matching R diagonal entry is
    // effectively zero (< QR_RANK_CUTOFF) and skip them.
    for (Size j = 0; j < s; ++j)
    {
        if (std::abs(rEig(static_cast<Eigen::Index>(j),
                          static_cast<Eigen::Index>(j))) < QR_RANK_CUTOFF)
        {
            continue;
        }
        for (Size i = 0; i < s; ++i)
        {
            ASSERT_NEAR(std::abs(qrRes._q(i, j)),
                        std::abs(qEig(static_cast<Eigen::Index>(i),
                                      static_cast<Eigen::Index>(j))),
                        CONSTEIG_TEST_TOLERANCE);
        }
    }
}
