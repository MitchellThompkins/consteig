#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.4862191863358021e-02, 2.4287958145141602e+00},
    {4.7410815954208374e-01, -5.4442590475082397e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
