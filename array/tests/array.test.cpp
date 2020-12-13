#include <gtest/gtest.h>
#include "array.hpp"

using namespace consteig;

TEST(array1d, static)
{
    static constexpr int s {3};
    const Array<int, s> array {0, 1, 2};

    int j = 0;
    for (auto i : array)
    {
        ASSERT_EQ(j, i);
        j++;
    }

    j = 0;
    for (const auto& i : array)
    {
        ASSERT_EQ(j, i);
        j++;
    }

    j = 0;
    for( size_t i {0}; i<s; i++ )
    {
        ASSERT_EQ(j,array[i]);
        j++;
    }
}

TEST(array2d, static)
{

    static constexpr int s {2};
    const Array<Array<int, s>, s> array {{{0, 1}, {2, 3}}};

    int m = 0;
    for(auto i : array)
    {
        for(auto j : i)
        {
            ASSERT_EQ(m, j);
            m++;
        }
    }

    m = 0;
    for(const auto& i : array)
    {
        for(auto j : i)
        {
            ASSERT_EQ(m, j);
            m++;
        }
    }

    m = 0;
    for( size_t i {0}; i<s; i++ )
    {
        for( size_t j {0}; j<s; j++ )
        {
            ASSERT_EQ(m,array[i][j]);
            m++;
        }
    }
}

TEST(array, size)
{
    const int array_size = 100;
    Array<int, array_size> array = {};
    ASSERT_EQ(array.size(), array_size);
}
