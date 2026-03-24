#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.0531505822984815e+00, -1.3825628515337191e-01},
    {4.0486815217878147e-01, 9.4684941770151931e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
