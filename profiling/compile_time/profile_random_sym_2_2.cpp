#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-8.0559533834457397e-01, -7.0631538331508636e-01},
    {-7.0631538331508636e-01, 2.6422772407531738e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
