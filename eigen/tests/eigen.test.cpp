#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "../consteig.hpp"
#include "eigen_test_tools.hpp"

using namespace consteig;

TEST(consteig_eigen, symmetric_matrix)
{
    static constexpr Size s{5};

    static constexpr consteig::Matrix<double, s, s> mat{{{
        {-5, -4, 2, 1, 77.1},
        {-4, 5, 7, 8, 9.2},
        {2, 7, 0, -83, 2},
        {1, 8, -83, 3, 4},
        {77.1, 9.2, 2, 4, 2},
    }}};

    // Calculate using consteig (returns Complex<double>)
    static constexpr auto eigenValueTest{eigvals(mat)};

    // Calculate using Eigen (Reference)
    auto eigenMat = toEigen(mat);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double, s, s>> es(eigenMat);
    auto eigenValsRef = es.eigenvalues(); // Returns sorted vector

    // Copy consteig results to std::vector for sorting (extract real part since
    // symmetric)
    std::vector<double> myVals;
    for (Size i = 0; i < s; ++i)
    {
        EXPECT_NEAR(eigenValueTest(i, 0).imag, 0.0,
                    CONSTEIG_TEST_TOLERANCE); // Symmetric -> real eigenvalues
        myVals.push_back(eigenValueTest(i, 0).real);
    }
    std::sort(myVals.begin(), myVals.end());

    // Compare
    for (Size i = 0; i < s; ++i)
    {
        EXPECT_NEAR(myVals[i], eigenValsRef[static_cast<Eigen::Index>(i)],
                    CONSTEIG_TEST_TOLERANCE);
    }
}
