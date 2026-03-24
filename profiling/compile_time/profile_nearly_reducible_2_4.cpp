#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-7.6141959428787231e-01, 7.3024982213973999e-01},
    {1.1083965003490450e-06, -1.5129083395004272e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
