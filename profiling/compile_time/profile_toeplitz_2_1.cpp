#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.1583853960037231e+00, 5.3370612859725952e-01},
    {5.3370612859725952e-01, -1.1583853960037231e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
