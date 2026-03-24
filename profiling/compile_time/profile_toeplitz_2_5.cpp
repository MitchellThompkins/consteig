#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.1102383136749268e+00, -1.0429614782333374e+00},
    {-1.0429614782333374e+00, 1.1102383136749268e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
