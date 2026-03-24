#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.2292658216054202e+00, -9.9897899536933943e-01},
    {1.0210046306604735e-03, -1.1653921281016111e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
