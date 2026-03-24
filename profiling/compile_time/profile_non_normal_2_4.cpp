#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.5401214897404982e+00, -3.0449210412260924e-01},
    {-8.1575273282286564e-01, 1.4598785102595018e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
