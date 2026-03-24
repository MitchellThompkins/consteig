#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.0750404438560364e+00, -5.0957119298193365e-01},
    {2.1851159648874057e+00, 9.2495955614396430e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
