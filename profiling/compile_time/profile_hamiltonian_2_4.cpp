#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-8.2611197233200073e-01, -3.2526977062225342e+00},
    {-1.9517698884010315e-01, 8.2611197233200073e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
