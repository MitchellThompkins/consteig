#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.6701760085668351e+01, -6.7088603164488561e+00},
    {3.3613475080653860e-03, 5.0056155051887538e+02}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
