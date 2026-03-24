#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.2874899208545685e-01, -1.3383185863494873e+00},
    {-1.8911883831024170e+00, -3.1242021918296814e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
