#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.6602100379341098e+00, 8.2064107654075991e-01},
    {-1.2183581866564239e+00, 3.3978996206589041e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
