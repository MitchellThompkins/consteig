#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-8.2208622235554643e+01, -3.2650903278773877e+00},
    {-2.4033426289859921e+02, 1.6099742587905074e-03}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
