#include "../../consteig.hpp"

using namespace consteig;

static constexpr Matrix<double, 2, 2> mat
{{{
    {1.1877301931381226e+00, 2.4118828773498535e+00},
    {1.2866750955581665e+00, -1.1877301931381226e+00}
}}};

static constexpr auto result = eigvals(mat);

int main() { return 0; }
