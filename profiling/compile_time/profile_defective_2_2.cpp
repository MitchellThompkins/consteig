#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.2074577103717226e+00, -9.7439196737517697e-01},
    {2.5608032624822637e-02, -8.9153201474119215e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
