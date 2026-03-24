#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.0617247823941316e+00, -1.1266614323746620e+00},
    {-1.2949669486176429e+00, 9.3827521760586852e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
