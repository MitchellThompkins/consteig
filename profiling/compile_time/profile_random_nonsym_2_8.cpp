#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-5.5441457033157349e-01, 1.0385729074478149e+00},
    {1.8879454135894775e+00, 2.8228014707565308e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
