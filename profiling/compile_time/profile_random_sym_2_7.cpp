#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-4.4505131244659424e-01, -2.1194067597389221e+00},
    {-2.1194067597389221e+00, -3.5238547325134277e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
