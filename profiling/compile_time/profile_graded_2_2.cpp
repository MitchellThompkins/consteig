#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {8.1263097881767088e+00, -2.0619320157018841e-03},
    {-8.3638524186607199e+00, 5.8951666870855899e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
