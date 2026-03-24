#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {2.7555045485496521e-01, 1.9031351804733276e-01},
    {-3.2115331292152405e-01, 3.2386537641286850e-02}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
