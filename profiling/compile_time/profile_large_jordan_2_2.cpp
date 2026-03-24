#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.0000000000000000e+00, 1.0000000000000000e+00},
    {0.0000000000000000e+00, 2.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
