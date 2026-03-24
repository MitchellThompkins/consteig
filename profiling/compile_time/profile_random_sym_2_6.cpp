#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-2.0421700477600098e+00, -3.8364142179489136e+00},
    {-3.8364142179489136e+00, 2.3614394664764404e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
