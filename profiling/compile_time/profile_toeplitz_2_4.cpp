#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {4.1993021965026855e-01, 3.1217044591903687e-01},
    {3.1217044591903687e-01, 4.1993021965026855e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
