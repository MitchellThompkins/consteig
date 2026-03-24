#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-8.7765254080295563e-02, 5.0493836402893066e-01},
    {-4.3847513198852539e-01, 1.9449463486671448e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
