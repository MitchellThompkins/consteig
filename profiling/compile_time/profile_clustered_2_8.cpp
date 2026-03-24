#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.0000099994515794e+00, 7.4053390449440982e-08},
    {7.4053390449406426e-08, 1.0000000005484204e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
