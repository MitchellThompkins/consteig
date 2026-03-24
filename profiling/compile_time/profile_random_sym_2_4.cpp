#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.2800428867340088e-01, 4.2807559669017792e-01},
    {4.2807559669017792e-01, 1.2920093536376953e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
