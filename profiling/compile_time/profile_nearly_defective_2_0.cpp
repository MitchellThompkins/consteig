#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {6.8449459316396621e-01, -5.8358396973978308e-02},
    {9.4164160302602185e-01, 2.1565477200174465e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
