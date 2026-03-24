#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000000000002e+00, 2.1309076325889811e-16},
    {2.1309076325889811e-16, 1.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
