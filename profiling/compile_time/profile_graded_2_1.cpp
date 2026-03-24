#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.7416271449655934e+01, -3.4989359863294603e-03},
    {-1.2976956168824989e+02, -3.1416005557267633e-03}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
