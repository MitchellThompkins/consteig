#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.6612859964370728e-01, -1.2234306335449219e+00},
    {4.6869903802871709e-06, -3.6345675587654114e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
