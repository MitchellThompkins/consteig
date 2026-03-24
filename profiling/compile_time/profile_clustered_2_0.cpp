#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000075684341334e+00, -4.2898888217831482e-06},
    {-4.2898888217782041e-06, 1.0000024315658664e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
