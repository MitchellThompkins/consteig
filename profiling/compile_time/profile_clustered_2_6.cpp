#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000055855449970e+00, 4.9655953376836952e-06},
    {4.9655953377150134e-06, 1.0000044144550029e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
