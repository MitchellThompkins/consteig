#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.1761994361877441e+00, 2.6115254163742065e+00},
    {2.6115254163742065e+00, 2.4756453037261963e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
