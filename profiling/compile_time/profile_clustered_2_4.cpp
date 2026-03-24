#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000089939009611e+00, -3.0081148768189591e-06},
    {-3.0081148767909731e-06, 1.0000010060990383e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
