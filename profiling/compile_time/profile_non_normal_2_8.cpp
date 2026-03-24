#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.6520229051083277e+00, 1.8119940164476878e+00},
    {1.2521511344018002e-01, 1.3479770948916729e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
