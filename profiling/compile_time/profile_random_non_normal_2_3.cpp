#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.7459905240565878e-01, 9.1384511347872655e-01},
    {-8.8371099624770322e-01, 2.4254009475943419e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
