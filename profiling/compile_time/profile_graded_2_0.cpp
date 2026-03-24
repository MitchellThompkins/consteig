#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-6.0424443326565964e+01, 2.1734453060319736e-02},
    {-3.3923797043600057e-03, 4.5946608157181931e-03}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
