#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {4.8086456539962280e-01, 1.2192136835262437e+00},
    {1.9536830894204585e-01, 2.5191354346003769e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
