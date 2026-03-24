#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {9.3529462814331055e-01, 0.0000000000000000e+00},
    {6.1276870965957642e-01, 0.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
