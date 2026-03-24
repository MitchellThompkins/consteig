#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.1908189058303833e-01, 1.3217450678348541e-01},
    {1.3217450678348541e-01, 5.1908189058303833e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
