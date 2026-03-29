#include "../math/complex.hpp"
#include "matrix.hpp"
#include <gtest/gtest.h>

using namespace consteig;

TEST(matrix, complex_init_minimal)
{
    // To initialize Matrix<T, R, C> via aggregate braces in C++17,
    // we need 4 levels of nesting to reach the elements:
    // 1. Matrix {
    // 2.   _data (T[R][C]) {
    // 3.     _data[R][C] (row T[C]) {
    // 4.       T (element) {
    //            re, im
    // 4.       }
    // 3.     }
    // 2.   }
    // 1. }
    static constexpr Matrix<Complex<double>, 1, 2> mat{
        {{{1.0, 3.0}, {2.0, 3.0}}}};

    static_assert(mat(0, 0).real == 1.0, "real part match");
    static_assert(mat(0, 0).imag == 3.0, "imag part match");
    static_assert(mat(0, 1).real == 2.0, "real 2 match");
    static_assert(mat(0, 1).imag == 3.0, "imag 2 match");
}

TEST(matrix, complex_init_lambda)
{
    // A much more readable alternative for larger matrices is using
    // a constexpr lambda to initialize the matrix iteratively.
    static constexpr Matrix<Complex<double>, 1, 2> mat = []() {
        Matrix<Complex<double>, 1, 2> m{};
        m(0, 0) = {1.0, 3.0};
        m(0, 1) = {2.0, 3.0};
        return m;
    }();

    static_assert(mat(0, 0).real == 1.0, "real part match");
    static_assert(mat(0, 1).real == 2.0, "real 2 match");
}

TEST(matrix, complex_init_braces_2x2)
{
    // For a 2x2 matrix, 4 levels of braces are required.
    static constexpr Matrix<Complex<double>, 2, 2> mat = {
        {{{1.0, 2.0}, {3.0, 4.0}}, {{5.0, 6.0}, {7.0, 8.0}}}};

    static_assert(mat(0, 0).real == 1.0, "00r");
    static_assert(mat(0, 1).real == 3.0, "01r");
    static_assert(mat(1, 0).real == 5.0, "10r");
    static_assert(mat(1, 1).imag == 8.0, "11i");
}
