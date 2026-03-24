#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {-1.8939767126044346e-01, 2.1161191761647200e-03},
    {5.8143713570511996e-04, -6.6919834118695648e-03}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
