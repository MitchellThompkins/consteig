#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.3127904601968396e+00, -1.9153996910069046e-01},
    {-1.1222335955044993e+00, 1.6872095398031606e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
