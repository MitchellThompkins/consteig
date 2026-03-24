#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.4313788703793529e-01, -9.8708917968827103e-01},
    {1.2910820311728955e-02, 1.7358073674497522e-02}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
