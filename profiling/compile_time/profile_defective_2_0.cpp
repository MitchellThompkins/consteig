#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.6935481325022039e+00, -4.2028293360556801e-02},
    {9.5797170663944320e-01, -1.2922406896718683e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
