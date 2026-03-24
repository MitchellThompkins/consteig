#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {9.9999999999999989e-01, 2.9612508006916206e-17},
    {2.9612508006916206e-17, 1.0000000000000000e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
