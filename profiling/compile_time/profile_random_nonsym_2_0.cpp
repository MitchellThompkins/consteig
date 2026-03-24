#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.2757926285266876e-02, -5.3299009799957275e-01},
    {2.1621541678905487e-01, -3.1870341300964355e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
