#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.2706577777862549e+00, -1.7988610267639160e+00},
    {-1.7988610267639160e+00, -1.2706577777862549e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
