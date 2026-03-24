#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-9.2746073007583618e-01, 3.1784455776214600e+00},
    {-9.1400164365768433e-01, 9.2746073007583618e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
