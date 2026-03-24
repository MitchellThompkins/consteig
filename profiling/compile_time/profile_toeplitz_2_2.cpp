#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.5294909477233887e-01, -7.5636982917785645e-01},
    {-7.5636982917785645e-01, -1.5294909477233887e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
