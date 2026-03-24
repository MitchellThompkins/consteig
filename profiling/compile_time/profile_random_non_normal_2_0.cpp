#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.9976437117090486e-01, -1.8836959469660497e-01},
    {7.9515678417340530e-01, 2.4002356288290949e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
