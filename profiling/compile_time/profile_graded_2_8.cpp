#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.7304604921004824e+02, 1.8571363416661524e-01},
    {3.4129620786386827e-01, -2.9121285664072487e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
