#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.4997663497924805e+00, 2.8920745290815830e-01},
    {2.8920745290815830e-01, -5.5277091264724731e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
