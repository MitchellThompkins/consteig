#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-9.1048288345336914e-01, 1.4831353425979614e+00},
    {-7.0401680469512945e-06, 4.9183470010757446e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
