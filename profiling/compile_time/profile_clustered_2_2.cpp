#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000015204036190e+00, -3.5906000926520967e-06},
    {-3.5906000926770473e-06, 1.0000084795963811e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
