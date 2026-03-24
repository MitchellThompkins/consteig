#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.4156719861819067e+00, -2.3194022940054876e-01},
    {-1.0472042159880730e+00, 1.5843280138180931e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
