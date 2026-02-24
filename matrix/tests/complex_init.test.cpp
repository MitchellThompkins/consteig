#include <gtest/gtest.h>
#include "matrix.hpp"
#include "../math/complex.hpp"

using namespace consteig;

TEST(matrix, complex_init_minimal)
{
    // Try explicit construction for one element
    static constexpr Complex<double> c1{1.0, 2.0};
    static constexpr Complex<double> c2{3.0, 4.0};
    
    // Matrix 1x2 -> Matrix<T, R, C> -> Array<Array<T, C>, R>
    // Matrix has one member _data which is Array<Array<T, C>, R>
    // Array has one member _data which is T[N]
    static constexpr Matrix<Complex<double>, 1, 2> mat{{{{{{1.0, 2.0}, {3.0, 4.0}}}}}};
    
    static_assert(mat(0, 0).real == 1.0, "real");
    static_assert(mat(0, 0).imag == 2.0, "imag");
}

TEST(matrix, complex_init_braces)
{
    // Matrix 2x2
    static constexpr Matrix<Complex<double>, 2, 2> mat = {
        {{{{{{1.0, 2.0}, {3.0, 4.0}}},
           {{{5.0, 6.0}, {7.0, 8.0}}}}}}
    };

    static_assert(mat(0, 0).real == 1.0, "00r");
    static_assert(mat(1, 1).imag == 8.0, "11i");
}
