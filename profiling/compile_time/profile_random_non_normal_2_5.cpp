#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.6834243045971271e+00, -5.6142447887613256e-01},
    {-4.4406020640088995e-01, 1.3165756954028733e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
