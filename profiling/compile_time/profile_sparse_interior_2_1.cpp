#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {0.0000000000000000e+00, -6.1079233884811401e-01},
    {0.0000000000000000e+00, -1.2078677415847778e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
