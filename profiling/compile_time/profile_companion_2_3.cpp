#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-9.0506603528756302e-01, -4.0798273749432665e+00},
    {1.0000000000000000e+00, 0.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
