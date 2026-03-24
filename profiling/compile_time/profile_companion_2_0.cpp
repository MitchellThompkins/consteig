#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {4.2259634695331216e-01, -1.2218272659827030e+00},
    {1.0000000000000000e+00, 0.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
