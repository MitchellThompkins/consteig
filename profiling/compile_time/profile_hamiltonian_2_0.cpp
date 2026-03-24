#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-4.7007378935813904e-01, 7.4191027879714966e-01},
    {1.1171669960021973e+00, 4.7007378935813904e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
