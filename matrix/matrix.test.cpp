#include <gtest/gtest.h>
#include "matrix.hpp"

TEST(matrix, static)
{
    const Matrix<int, 2, 2> matrix {{{{3, 5}, {2, 5}}}};
}
