#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "eigen_test_tools.hpp"

#include "../consteig.hpp"

using namespace consteig;

static constexpr float kThreshEigen {0.00001F};

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

    // Calculate using consteig (returns Complex<double>)
    static constexpr auto eigenValueTest {eigvals(mat)};

    // Calculate using Eigen (Reference)
    auto eigenMat = toEigen(mat);
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix<double, s, s>> es(eigenMat);
    auto eigenValsRef = es.eigenvalues(); // Returns sorted vector

    // Copy consteig results to std::vector for sorting (extract real part since symmetric)
    std::vector<double> myVals;
    for(size_t i=0; i<s; ++i) {
        EXPECT_NEAR(eigenValueTest(i,0).imag, 0.0, 1e-9); // Symmetric -> real eigenvalues
        myVals.push_back(eigenValueTest(i,0).real);
    }
    std::sort(myVals.begin(), myVals.end());

    // Compare
    for(size_t i=0; i<s; ++i) {
        EXPECT_NEAR(myVals[i], eigenValsRef[i], 1e-4) << "Mismatch at index " << i;
    }

    // static constexpr bool checkEigen = checkEigenValues<double,s,s>(mat, eigenValueTest, kThreshEigen);
    // static_assert(checkEigen==true, MSG);
    // ASSERT_TRUE( checkEigen );
}

TEST(consteig_eigen, non_symmetric_complex_eigenvalues)
{
    static constexpr size_t s {2};
    // Matrix with complex eigenvalues: [0, 1; -1, 0] -> +/- i
    static constexpr Matrix<double, s, s> mat
    {{{
        {0.0, 1.0},
        {-1.0, 0.0}
    }}};

    static constexpr auto eigenValueTest {eigvals(mat)};
    
    // Check results
    bool found_i = false;
    bool found_neg_i = false;
    
    for(size_t i=0; i<s; ++i) {
        Complex<double> val = eigenValueTest(i,0);
        EXPECT_NEAR(val.real, 0.0, 1e-5);
        if (std::abs(val.imag - 1.0) < 1e-5) found_i = true;
        if (std::abs(val.imag + 1.0) < 1e-5) found_neg_i = true;
    }
    EXPECT_TRUE(found_i);
    EXPECT_TRUE(found_neg_i);
}

TEST(consteig_eigen, non_symmetric_general)
{
    static constexpr size_t s {3};
    static constexpr Matrix<double, s, s> mat
    {{{
        {1.0, 2.0, 3.0},
        {0.0, 1.0, 5.0}, // Upper triangular part
        {0.0, -2.0, 1.0} // Block [1, 5; -2, 1] at bottom right? No.
        // Let's use a known matrix or random non-symmetric.
        // Actually the matrix above:
        // {1, 2, 3}
        // {0, 1, 5}
        // {0, -2, 1}
        // Eigenvalues: 1 (from top left), and eigenvalues of [1, 5; -2, 1].
        // [1, 5; -2, 1] -> (1-L)(1-L) - (-10) = 0 -> (1-L)^2 + 10 = 0 -> (1-L)^2 = -10 -> 1-L = +/- i sqrt(10) -> L = 1 +/- i sqrt(10).
    }}};
    
    // 1 +/- i * 3.162
    
    static constexpr auto eigenValueTest {eigvals(mat)};
    
    std::vector<Complex<double>> vals;
    for(size_t i=0; i<s; ++i) vals.push_back(eigenValueTest(i,0));
    
    // Sort logic for complex is tricky, just search
    bool found_1 = false;
    bool found_c1 = false;
    bool found_c2 = false;
    
    for(const auto& v : vals) {
        if (std::abs(v.real - 1.0) < 1e-4) {
            if (std::abs(v.imag) < 1e-4) found_1 = true;
            else if (std::abs(v.imag - std::sqrt(10.0)) < 1e-4) found_c1 = true;
            else if (std::abs(v.imag + std::sqrt(10.0)) < 1e-4) found_c2 = true;
        }
    }
    
    EXPECT_TRUE(found_1);
    EXPECT_TRUE(found_c1);
    EXPECT_TRUE(found_c2);
}
