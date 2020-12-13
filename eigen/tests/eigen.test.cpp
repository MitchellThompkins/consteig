#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "../consteig.hpp"

using namespace consteig;

static constexpr float kThreshEigen {0.00001F};

template<typename T, size_t R, size_t C>
static inline constexpr bool checkEigenValues(
        const Matrix<T,R,C> a,
        const Matrix<T,R,1> lambda,
        const T thresh)
{
    //det(A-lambda*I)
    bool equal {true};
    auto identity {eye<T,R>()};

    for(int i {0}; i<R; i++)
    {
        equal &= compareFloat( det( a - (lambda(i,0)*identity)), static_cast<T>(0), thresh );
    }

    return equal;
}

TEST(solver, constexpr_eigenValues)
{
    static constexpr size_t x {4};

    static constexpr Matrix<double,x,x> mat
    {{{
        { -4.4529e-01,  4.9063e+00, -8.7871e-01,  6.3036e+00},
        { -6.3941e+00,  1.3354e+01,  1.6668e+00,  1.1945e+01},
        {  3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
        {  3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}
    }}};

    static constexpr auto eigenValueTest {eigvals(mat)};

    static constexpr bool checkEigen = checkEigenValues<double,x,x>(mat, eigenValueTest, kThreshEigen);
    static_assert(checkEigen==true, MSG);
    ASSERT_TRUE( checkEigen );
}
