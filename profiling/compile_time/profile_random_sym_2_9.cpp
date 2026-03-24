#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.0667097568511963e+00, 9.5756608247756958e-01},
    {9.5756608247756958e-01, -1.7083342075347900e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
