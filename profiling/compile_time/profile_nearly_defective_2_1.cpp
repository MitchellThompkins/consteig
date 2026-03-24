#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {9.7120228177648915e-01, -3.7163486320059208e-01},
    {6.2836513679940786e-01, 4.7191900367127343e-03}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
