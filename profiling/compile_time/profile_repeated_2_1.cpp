#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000000000000004e+00, 4.4501141363275007e-16},
    {4.4501141363275007e-16, 1.0000000000000004e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
