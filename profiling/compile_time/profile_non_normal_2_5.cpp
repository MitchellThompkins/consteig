#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0470293318430592e+00, -9.4082378085111246e-02},
    {-4.7636523121163710e-01, 1.9529706681569408e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
