#include <gtest/gtest.h>
#include "matrix.hpp"

TEST(matrix, static)
{
    static constexpr int s {2};
    static constexpr Matrix<int, s, s> matrix {{{{0, 1}, {2, 3}}}};

    // Check that created objects are constexpr
    static_assert(matrix(0,0) == 0, "Not constexpr");
    static_assert(matrix(0,1) == 1, "Not constexpr");
    static_assert(matrix(1,0) == 2, "Not constexpr");
    static_assert(matrix(1,1) == 3, "Not constexpr");

    int m = 0;
    for( size_t i {0}; i<s; i++ )
    {
        for( size_t j {0}; j<s; j++ )
        {
            ASSERT_EQ(m, matrix(i,j));
            m++;
        }
    }
}

TEST(matrix, equality)
{
    static constexpr int s {2};
    static constexpr Matrix<int, s, s> mat1 {{{{0, 1}, {2, 3}}}};
    static constexpr Matrix<int, s, s> mat2 {{{{0, 1}, {2, 3}}}};
    static constexpr Matrix<int, s, s> mat3 {{{{3, 2}, {1, 0}}}};

    // Check that created objects are constexpr
    static_assert(mat1==mat2, "Not constexpr");
    static_assert(mat2!=mat3, "Not constexpr");

    ASSERT_TRUE(mat1==mat2);
    ASSERT_FALSE(mat2==mat3);

    ASSERT_TRUE(mat1!=mat3);
    ASSERT_FALSE(mat1!=mat2);
}

TEST(matrix, static_constexpr_addition)
{
    static constexpr int s {2};
    static constexpr Matrix<int, s, s> mat1 {{{{3, 4}, {5, 6}}}};
    static constexpr Matrix<int, s, s> mat2 {{{{1, 3}, {2, 5}}}};

    static constexpr Matrix<int, s, s> answer {{{{4, 7}, {7, 11}}}};

    static constexpr Matrix<int, s, s> test {mat1 + mat2};


    for( size_t i {0}; i<s; i++ )
    {
        for( size_t j {0}; j<s; j++ )
        {
            ASSERT_EQ( test(i,j), answer(i,j) );
        }
    }
}

TEST(matrix, regular_addition)
{
    static constexpr int r {3};
    static constexpr int c {2};

    Matrix<int, r, c> mat1 {{{{3, 4}, {5, 6}, {3, 9}}}};
    Matrix<int, r, c> mat2 {{{{1, 3}, {2, 5}, {1, 10}}}};

    static constexpr Matrix<int, r, c> addAnswer
    {{{{4, 7}, {7, 11}, {4, 19}}}};
    static constexpr Matrix<int, r, c> subAnswer
    {{{{2, 1}, {3, 1}, {2, -1}}}};

    Matrix<int, r, c> add {mat1 + mat2};
    Matrix<int, r, c> sub {mat1 - mat2};

    for( size_t i {0}; i<r; i++ )
    {
        for( size_t j {0}; j<c; j++ )
        {
            ASSERT_EQ( add(i,j), addAnswer(i,j) );
            ASSERT_EQ( sub(i,j), subAnswer(i,j) );
        }
    }
}

TEST(matrix, static_constexpr_multiplication)
{
    static constexpr int r {3};
    static constexpr int c {2};

    static constexpr Matrix<int, r, c> mat1 {{{{3, 4}, {5, 6}, {3, 9}}}};
    static constexpr Matrix<int, c, r> mat2 {{{{1, -3, 7}, {2, 5, 9}}}};

    static constexpr Matrix<int, r, r> answer1
    {{{{11, 11, 57}, {17, 15, 89}, {21, 36, 102}}}};

    static constexpr Matrix<int, c, c> answer2
    {{{{9, 49}, {58, 119}}}};

    static constexpr Matrix<int, r, r> mult1 { mat1*mat2 };
    static constexpr Matrix<int, c, c> mult2 { mat2*mat1 };
    
    for( size_t i {0}; i<r; i++ )
    {
        for( size_t j {0}; j<r; j++ )
        {
            ASSERT_EQ( mult1(i,j), answer1(i,j) );
        }
    }

    for( size_t i {0}; i<c; i++ )
    {
        for( size_t j {0}; j<c; j++ )
        {
            ASSERT_EQ( mult2(i,j), answer2(i,j) );
        }
    }

}
