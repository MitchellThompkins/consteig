#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.2341969609260559e-01, 8.5905838012695312e-01},
    {-5.9512499719858176e-07, 1.1443912982940674e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
