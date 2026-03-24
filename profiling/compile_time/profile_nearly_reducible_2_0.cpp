#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.5999088287353516e+00, 2.5713130831718445e-01},
    {-3.1029516458511356e-06, 8.4395855665206909e-02}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
