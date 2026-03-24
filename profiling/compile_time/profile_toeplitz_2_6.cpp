#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.4759469032287598e-01, -1.0624668598175049e+00},
    {-1.0624668598175049e+00, -1.4759469032287598e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
