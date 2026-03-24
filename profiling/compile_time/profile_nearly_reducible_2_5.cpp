#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0735219717025757e-01, -1.1795334815979004e+00},
    {2.2536168992519380e-06, 1.2565647363662720e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
