#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-5.4917387629549605e-02, -6.6086601834262715e-01},
    {3.3913398165737269e-01, -1.0017481521119664e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
