#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-9.3872687249624540e-02, -2.4040946236773728e-01},
    {7.5959053763226270e-01, 7.6079158080431197e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
