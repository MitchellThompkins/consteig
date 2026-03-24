#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.1418328285217285e-01, -9.5765882730484009e-01},
    {1.0962281227111816e+00, -5.1418328285217285e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
