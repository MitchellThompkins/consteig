#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.7351951010428560e-02, 1.6119955483286246e+00},
    {3.2865460138982710e-04, 5.9037787899471356e-03}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
