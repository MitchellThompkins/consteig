#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000659055603e+00, 8.0914279514926238e-07},
    {8.0914279515017463e-07, 1.0000099340944395e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
