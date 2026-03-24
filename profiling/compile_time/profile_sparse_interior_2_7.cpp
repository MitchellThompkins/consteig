#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.3330867886543274e-01, -6.2160360813140869e-01},
    {0.0000000000000000e+00, 0.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
