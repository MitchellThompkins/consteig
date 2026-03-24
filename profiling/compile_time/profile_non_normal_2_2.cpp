#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.4240947802411250e+00, 5.7251149240884414e-01},
    {4.2660872463140115e-01, 1.5759052197588745e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
