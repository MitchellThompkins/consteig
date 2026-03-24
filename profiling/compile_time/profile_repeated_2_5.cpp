#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000000000000e+00, -2.6639901663758771e-17},
    {-2.6639901663758771e-17, 1.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
