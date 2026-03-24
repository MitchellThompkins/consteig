#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {4.4608449935913086e-01, -1.6870151758193970e+00},
    {1.0473135709762573e+00, 1.3415925502777100e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
