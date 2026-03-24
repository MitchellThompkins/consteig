#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.0063780537331637e+00, -9.9077249566945580e-01},
    {9.2275043305443706e-03, -1.1976094057089750e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
