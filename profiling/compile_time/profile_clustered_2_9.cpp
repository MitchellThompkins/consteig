#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000073968286021e+00, 4.3880761904502029e-06},
    {4.3880761904389382e-06, 1.0000026031713978e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
