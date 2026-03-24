#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000095270588409e+00, 2.1226724317508228e-06},
    {2.1226724317408155e-06, 1.0000004729411593e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
