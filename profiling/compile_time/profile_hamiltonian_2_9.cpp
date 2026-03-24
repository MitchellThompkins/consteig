#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.5730341672897339e-01, -2.9695770740509033e+00},
    {7.8976309299468994e-01, 2.5730341672897339e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
