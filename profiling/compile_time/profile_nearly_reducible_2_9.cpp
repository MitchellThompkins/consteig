#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {9.6391779184341431e-01, -8.2952016592025757e-01},
    {1.4041341841220858e-06, -2.6364094018936157e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
