#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.1465821266174316e+00, 6.0005068778991699e-01},
    {-9.6257269382476819e-06, 6.4247637987136841e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
