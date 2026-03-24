#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.1357948780059814e+00, 2.5029866695404053e+00},
    {1.3078796863555908e-01, -2.1357948780059814e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
