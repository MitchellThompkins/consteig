#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.6864800453186035e-01, 0.0000000000000000e+00},
    {0.0000000000000000e+00, 1.4972099065780640e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
