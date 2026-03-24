#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.2453162469154018e+00, 6.0461355650245208e-01},
    {2.6269930505572803e-01, 1.7546837530845987e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
