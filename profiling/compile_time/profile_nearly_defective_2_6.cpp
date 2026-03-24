#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-3.5734548037837288e-01, -4.9588010677242994e-01},
    {5.0411989322757012e-01, 6.4262057200549372e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
