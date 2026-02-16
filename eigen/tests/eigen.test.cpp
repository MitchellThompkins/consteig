#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <vector>
#include <algorithm>
#include "test_tools.hpp"

#include "../consteig.hpp"

using namespace consteig;

static constexpr float kThreshEigen {0.00001F};

template<typename T, size_t R, size_t C>
Eigen::Matrix<T, R, C> toEigen(const consteig::Matrix<T, R, C>& mat) {
    Eigen::Matrix<T, R, C> res;
    for(size_t i = 0; i < R; ++i) {
        for(size_t j = 0; j < C; ++j) {
            res(i, j) = mat(i, j);
        }
    }
    return res;
}

TEST(consteig_eigen, constexpr_eigenValues)
{
    static constexpr size_t s {4};

    static constexpr Matrix<double,s,s> mat
    {{{
        { -4.4529e-01,  4.9063e+00, -8.7871e-01,  6.3036e+00},
        { -6.3941e+00,  1.3354e+01,  1.6668e+00,  1.1945e+01},
        {  3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
        {  3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}
    }}};

    static constexpr auto eigenValueTest {eigvals(mat)};

    static constexpr bool checkEigen = checkEigenValues<double,s,s>(mat, eigenValueTest, kThreshEigen);
    //TODO(mthompkins): Comment this back in
    //static_assert(checkEigen==true, MSG);
    //ASSERT_TRUE( checkEigen );
}

TEST(consteig_eigen, symmetric_matrix)
{
    static constexpr size_t s {5};

    static constexpr consteig::Matrix<double, s, s> mat
    {{{
        {-5,   -4,    2,   1,  77.1},
        {-4,    5,    7,   8,  9.2 },
        { 2,    7,    0, -83,  2 },
        { 1,    8,  -83,   3,  4 },
        { 77.1, 9.2,  2,   4,  2 },
    }}};

    // Calculate using consteig
    static constexpr auto eigenValueTest {eigvals(mat)};

    // Calculate using Eigen (Reference)
    auto eigenMat = toEigen(mat);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double, s, s>> es(eigenMat);
    auto eigenValsRef = es.eigenvalues(); // Returns sorted vector

    // Copy consteig results to std::vector for sorting
    std::vector<double> myVals;
    for(size_t i=0; i<s; ++i) myVals.push_back(eigenValueTest(i,0));
    std::sort(myVals.begin(), myVals.end());

    // Compare
    for(size_t i=0; i<s; ++i) {
        EXPECT_NEAR(myVals[i], eigenValsRef[i], 1e-4) << "Mismatch at index " << i;
    }

    static constexpr bool checkEigen = checkEigenValues<double,s,s>(mat, eigenValueTest, kThreshEigen);
    static_assert(checkEigen==true, MSG);
    ASSERT_TRUE( checkEigen );
}
