#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.6670707820848365e+00, -1.6356383554397025e-01},
    {-1.5328503131097357e+00, 1.3329292179151637e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
