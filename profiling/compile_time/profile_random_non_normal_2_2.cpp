#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.3815864036241368e+00, -1.7475256523115235e-01},
    {1.1597173492051882e+00, 1.6184135963758637e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
