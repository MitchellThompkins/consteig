#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-6.1766260862350464e-01, 1.2738070487976074e+00},
    {4.3557971715927124e-01, 3.5228184424340725e-04}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
