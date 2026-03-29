#include <cmath>
#include <gtest/gtest.h>

#include "constmath.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(complex, constexpr_ops)
{
    static constexpr Complex<double> c1{1.0, 2.0};
    static constexpr Complex<double> c2{3.0, 4.0};

    static constexpr auto sum = c1 + c2;
    static_assert(sum.real == 4.0 && sum.imag == 6.0, MSG);

    static constexpr auto diff = c2 - c1;
    static_assert(diff.real == 2.0 && diff.imag == 2.0, MSG);

    static constexpr auto prod = c1 * c2;
    // (1+2i)(3+4i) = 3 + 4i + 6i - 8 = -5 + 10i
    static_assert(prod.real == -5.0 && prod.imag == 10.0, MSG);

    static constexpr auto quot =
        Complex<double>{1.0, 1.0} / Complex<double>{1.0, 1.0};
    static_assert(quot.real == 1.0 && quot.imag == 0.0, MSG);
}

TEST(complex, abs_conj)
{
    static constexpr Complex<double> c{3.0, 4.0};
    static constexpr double m = abs(c);
    static_assert(m == 5.0, MSG);

    static constexpr auto con = conj(c);
    static_assert(con.real == 3.0 && con.imag == -4.0, MSG);
}

TEST(complex, scalar_ops)
{
    static constexpr Complex<double> c{1.0, 2.0};
    static constexpr auto s1 = 2.0 * c;
    static_assert(s1.real == 2.0 && s1.imag == 4.0, MSG);

    static constexpr auto s2 = c * 3.0;
    static_assert(s2.real == 3.0 && s2.imag == 6.0, MSG);
}
