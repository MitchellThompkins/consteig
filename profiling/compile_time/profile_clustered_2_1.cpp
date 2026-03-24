#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000019558034832e+00, -3.9664678954151327e-06},
    {-3.9664678953759540e-06, 1.0000080441965171e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
