#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.4777104854583740e+00, 1.8050976693630219e+00},
    {1.8050976693630219e+00, -2.1737346649169922e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
