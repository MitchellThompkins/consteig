#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-5.2427685630663159e-03, 1.7786847966834399e-01},
    {3.0281870523683180e-01, 1.1837904781534765e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
