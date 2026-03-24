#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.1984945419562904e-01, -2.5609434944523635e+00},
    {-4.1740330165063064e+01, 2.2858082357265357e+02}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
