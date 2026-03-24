#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.6537854391849862e+00, -1.8320576091749688e-01},
    {8.1679423908250348e-01, 8.8011519913593761e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
