#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "../consteig.hpp"

using namespace consteig;

static constexpr float kThreshEigen {0.00001F};

TEST(solver, constexpr_eigenValues)
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

TEST(solver, symmetric_matrix)
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

    static constexpr auto eigenValueTest {eigvals(mat)};

    static constexpr bool checkEigen = checkEigenValues<double,s,s>(mat, eigenValueTest, kThreshEigen);
    static_assert(checkEigen==true, MSG);
    ASSERT_TRUE( checkEigen );

}
