#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.0499142409644482e+00, 1.1938617523786232e+00},
    {-4.3895930421669879e-02, 9.5008575903555215e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
