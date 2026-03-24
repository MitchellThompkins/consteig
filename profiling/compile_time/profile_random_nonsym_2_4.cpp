#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-7.8263711929321289e-01, -1.0138126611709595e+00},
    {6.4928883314132690e-01, -1.2468265742063522e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
