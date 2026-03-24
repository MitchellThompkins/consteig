#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.8215805270090151e+00, -7.4256142271014247e-01},
    {2.5743857728985758e-01, 9.4713451675319227e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
