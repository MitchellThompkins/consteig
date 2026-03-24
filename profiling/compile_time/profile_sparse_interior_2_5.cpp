#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {0.0000000000000000e+00, 8.6421239376068115e-01},
    {0.0000000000000000e+00, 4.3852761387825012e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
