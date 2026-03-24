#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {6.1595335006713867e+00, -1.3090503513813019e+00},
    {-1.3090503513813019e+00, 2.0632321834564209e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
