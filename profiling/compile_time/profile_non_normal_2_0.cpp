#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.7073362827296592e+00, -6.2209323753862644e-01},
    {-3.3276630153208053e-01, 1.2926637172703408e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
