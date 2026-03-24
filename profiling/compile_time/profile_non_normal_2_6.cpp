#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.5821804313999506e+00, 4.7277975518191756e-01},
    {-1.9482951120666174e+00, 4.1781956860004993e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
