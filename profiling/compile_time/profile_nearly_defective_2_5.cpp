#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-7.4651185922717478e-01, -3.5587851791286229e-01},
    {6.4412148208713771e-01, 2.1104544642352485e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
