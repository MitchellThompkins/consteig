#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000000000002e+00, 2.7100634195322805e-17},
    {2.7100634195322805e-17, 9.9999999999999989e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
