#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.5345572419081828e+00, -8.0242931193008138e-01},
    {1.9757068806991882e-01, 7.3822436858070895e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
