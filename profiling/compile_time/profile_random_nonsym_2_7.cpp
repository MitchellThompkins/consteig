#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-3.2591539621353149e-01, 2.6841363310813904e-01},
    {1.2041262388229370e+00, -8.8272577524185181e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
