#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000000000002e+00, -8.5636223816355478e-17},
    {-8.5636223816355478e-17, 1.0000000000000002e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
