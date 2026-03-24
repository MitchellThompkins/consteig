#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {6.0871636867523193e-01, 5.7740181684494019e-01},
    {-2.1355891227722168e+00, -6.0871636867523193e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
