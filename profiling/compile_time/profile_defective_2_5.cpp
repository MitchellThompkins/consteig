#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.3597994690194173e+00, -3.9674447261554219e-01},
    {6.0325552738445753e-01, -1.3813552016959136e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
