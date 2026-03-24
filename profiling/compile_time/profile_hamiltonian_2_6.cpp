#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.3268036842346191e+00, 3.7725421786308289e-01},
    {6.6904628276824951e-01, 1.3268036842346191e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
