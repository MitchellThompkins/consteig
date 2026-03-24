#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000087240777065e+00, 3.3363520851956516e-06},
    {3.3363520852008367e-06, 1.0000012759222936e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
