#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.3122905960989479e-01, -4.4787719867214953e-01},
    {5.5212280132785052e-01, -7.6332252493811137e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
