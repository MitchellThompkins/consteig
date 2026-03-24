#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-6.4782893657684326e-01, 1.1695100069046021e+00},
    {2.0245178043842316e-06, -4.4905424118041992e-02}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
