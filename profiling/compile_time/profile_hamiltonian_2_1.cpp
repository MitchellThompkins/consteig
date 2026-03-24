#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.2450039386749268e+00, 4.2098310589790344e-01},
    {-4.3739315867424011e-01, -1.2450039386749268e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
