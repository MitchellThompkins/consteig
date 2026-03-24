#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.0903217549039470e-01, -4.2925303162363521e-01},
    {5.7074696837636485e-01, 1.4989712977914658e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
