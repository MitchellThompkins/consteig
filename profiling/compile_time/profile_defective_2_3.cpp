#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-3.2367679362419144e-01, -6.6569825464133703e-01},
    {3.3430174535866314e-01, 6.1981478219154362e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
