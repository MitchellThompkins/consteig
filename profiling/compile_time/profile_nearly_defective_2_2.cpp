#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.2072625382638550e-01, -7.7261901772096631e-01},
    {2.2738098227903369e-01, 6.1755501442933658e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
