#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-3.7094405743472875e-01, -5.9656320906972204e-01},
    {4.0343679093027762e-01, 6.1022982451312857e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
