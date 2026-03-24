#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {6.3753056526184082e-01, 1.7382221221923828e+00},
    {1.7382221221923828e+00, 6.3753056526184082e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
