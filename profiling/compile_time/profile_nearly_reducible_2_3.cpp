#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.0540918111801147e+00, 5.9672534465789795e-01},
    {-5.4030835628509525e-06, 3.0182015895843506e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
