#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {5.2672630548477173e-01, 4.9264276027679443e-01},
    {4.9264276027679443e-01, 5.2672630548477173e-01}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
