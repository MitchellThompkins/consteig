#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {9.2874723672866821e-01, -8.5560643672943115e-01},
    {-8.5560643672943115e-01, 9.2874723672866821e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
