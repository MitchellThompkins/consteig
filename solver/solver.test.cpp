#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "solver.hpp"
#include "stdint.h"
#include "gcem.hpp"

static constexpr float kThreshEigen {0.0009F};

template<typename T, size_t R, size_t C>
static inline constexpr bool checkEigenValues(
        const Matrix<T,R,C> a,
        const Matrix<T,R,1> lambda )
{
    //det(A-lambda*I)
    bool equal {true};
    auto identity {diagional<T,R>(static_cast<T>(1))};

    for(int i {0}; i<R; i++)
    {
        equal &= compareFloat( det( a - (lambda(i,0)*identity)), static_cast<T>(0), kThreshEigen );
    }

    return equal;
}

TEST(solver, static_constexpr_eigenValues)
{
    static constexpr size_t x {4};

    static constexpr Matrix<float,x,x> mat
    {{{
        { -4.4529e-01,  4.9063e+00, -8.7871e-01,  6.3036e+00},
        { -6.3941e+00,  1.3354e+01,  1.6668e+00,  1.1945e+01},
        {  3.6842e+00, -6.6617e+00, -6.0021e-02, -7.0043e+00},
        {  3.1209e+00, -5.2052e+00, -1.4130e+00, -2.8484e+00}
    }}};

    static constexpr auto eigenValueTest {eigenvalues(mat)};

    static constexpr bool checkEigen = checkEigenValues<float,x,x>(mat,eigenValueTest);
    //static_assert(checkEigen==true, MSG);
    //ASSERT_TRUE( checkEigen );
}
