#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.1318302154541016e+00, 2.5287854671478271e+00},
    {2.5287854671478271e+00, 1.4644197225570679e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
