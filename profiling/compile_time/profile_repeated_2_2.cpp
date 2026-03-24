#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000000000004e+00, -1.7779819817538290e-16},
    {-1.7779819817538290e-16, 1.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
