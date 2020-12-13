#include <gtest/gtest.h>
#include "test_tools.hpp"

#include "matrix.hpp"

using namespace consteig;

// These helper functions allow us to call templated constexpr functions
template<typename T, size_t R, size_t C>
static constexpr Matrix<T,R,C> setRowsTest(
        const Matrix<T,1,C> mat[],
        const T val )
{
    Matrix<T,R,C> out {};
    out.setRow(mat[0],val);
    out.setRow(mat[1],val+1);
    out.setRow(mat[2],val+2);
    return out;
}

template<typename T, size_t R, size_t C>
static constexpr Matrix<T,R,C> setColsTest(
        const Matrix<T,R,1> mat[],
        const T val )
{
    Matrix<T,R,C> out {};
    out.setCol(mat[0],val);
    out.setCol(mat[1],val+1);
    out.setCol(mat[2],val+2);
    return out;
}

template<typename T, size_t R, size_t C, size_t startIndex, size_t endIndex>
static constexpr Matrix<T,R,C> setSubRowTest(
        Matrix<T,R,C> original,
        const Matrix<T,1,endIndex-startIndex+1> mat,
        const size_t row )
{
    original.template setRow<startIndex,endIndex>(mat, row);
    return original;
}


template<typename T, size_t R, size_t C, size_t startIndex, size_t endIndex>
static constexpr Matrix<T,R,C> setSubColTest(
        Matrix<T,R,C> original,
        const Matrix<T, endIndex-startIndex+1, 1> mat,
        const size_t col )
{
    original.template setCol<startIndex,endIndex>(mat, col);
    return original;
}

template<typename T, size_t R, size_t C, size_t x1, size_t y1, size_t x2, size_t y2>
static constexpr Matrix<T,R,C> setSubTest(
        Matrix<T,R,C> original,
        const Matrix<T, x2-x1+1, y2-y1+1> mat )
{
    original.template setSub<x1, y1, x2, y2>(mat);
    return original;
}

TEST(matrix, static_constexpr)
{
    static constexpr int s {2};
    static constexpr Matrix<int, s, s> matrix {{{{0, 1}, {2, 3}}}};

    // Check that created objects are constexpr
    static_assert(matrix(0,0) == 0, MSG);
    static_assert(matrix(0,1) == 1, MSG);
    static_assert(matrix(1,0) == 2, MSG);
    static_assert(matrix(1,1) == 3, MSG);

    int m = 0;
    for( size_t i {0}; i<s; i++ )
    { for( size_t j {0}; j<s; j++ )
        {
            ASSERT_EQ(m, matrix(i,j));
            m++;
        }
    }
}

TEST(matrix, checkSize)
{
    static constexpr size_t x {2};
    static constexpr size_t y {3};

    static constexpr Matrix<int, x, y> matrix
    {{{ {5, -1, -2}, {-4, -2 , 1}}}};

    static constexpr size_t len    {matrix.sizeX()};
    static constexpr size_t height {matrix.sizeY()};

    // Check sizes
    static_assert(len == x, MSG);
    static_assert(height == y, MSG);

    ASSERT_EQ(len, x);
    ASSERT_EQ(height, y);
}

TEST(matrix, equality)
{
    static constexpr int s {2};
    static constexpr Matrix<int, s, s> mat1 {{{{0, 1}, {2, 3}}}};
    static constexpr Matrix<int, s, s> mat2 {{{{0, 1}, {2, 3}}}};
    static constexpr Matrix<int, s, s> mat3 {{{{3, 2}, {1, 0}}}};

    // Check that created objects are constexpr
    static_assert(mat1==mat2, MSG);
    static_assert(mat2!=mat3, MSG);

    ASSERT_TRUE(mat1==mat2);
    ASSERT_FALSE(mat2==mat3);

    ASSERT_TRUE(mat1!=mat3);
    ASSERT_FALSE(mat1!=mat2);
}

TEST(matrix, matrix_equals_operator)
{
    static constexpr int s {2};
    static constexpr Matrix<int, s, s> mat1 {{{{0, 1}, {2, 3}}}};
    static constexpr Matrix<int, s, s> mat2 = mat1;

    // Check that created objects are constexpr
    static_assert(mat1==mat2, MSG);

    ASSERT_TRUE(mat1==mat2);
}

TEST(matrix, static_constexpr_row)
{
    static constexpr int x {3};
    static constexpr int limit {1};

    static constexpr Matrix<float, x, x> mat
    {{{{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F , 3.0F}, {-2.0F, 1.0F, 0.0F}}}};

    static constexpr Matrix<float, 1, x> rowExtract {mat.row(0)};
    static constexpr Matrix<float, 1, x-limit> rowExtractLimit {mat.row<limit, x-1>(0)};

    static constexpr Matrix<float, 1, x> answerFull
    {{{{5.0F, -4.0F, 2.0F}}}};

    static constexpr Matrix<float, 1, x-limit> answerLimit
    {{{{-4.0F, 2.0F}}}};

    // Check that created objects are constexpr
    static_assert(rowExtract==answerFull, MSG);
    static_assert(rowExtractLimit==answerLimit, MSG);

    ASSERT_TRUE(rowExtract==answerFull);
    ASSERT_TRUE(rowExtractLimit==answerLimit);
}

TEST(matrix, static_constexpr_col)
{
    static constexpr int x {3};
    static constexpr int limit {1};

    static constexpr Matrix<float, x, x> mat
    {{{{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F , 3.0F}, {-2.0F, 1.0F, 0.0F}}}};

    static constexpr Matrix<float, x, 1> colExtract {mat.col(0)};
    static constexpr Matrix<float, x-limit, 1> colExtractLimit {mat.col<limit, x-1>(0)};

    static constexpr Matrix<float, x, 1> answerFull
    {{{{5.0F}, {-1.0F}, {-2.0F}}}};

    static constexpr Matrix<float, x-limit, 1> answerLimit
    {{{{-1.0F}, {-2.0F}}}};

    // Check that created objects are constexpr
    static_assert(colExtract==answerFull, MSG);
    static_assert(colExtractLimit==answerLimit, MSG);

    ASSERT_TRUE(colExtract==answerFull);
    ASSERT_TRUE(colExtractLimit==answerLimit);
}

TEST(matrix, static_constexpr_subMatrix)
{
    static constexpr int x {3};

    static constexpr Matrix<float, x, x> mat
    {{{{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F , 3.0F}, {-2.0F, 1.0F, 0.0F}}}};

    static constexpr Matrix<float, x-1, x-1> subMat {mat.sub<1,1,2,2>()};

    static constexpr Matrix<float, x-1, x-1> answer
    {{{{2.0F , 3.0F}, {1.0F, 0.0F}}}};

    // Check that created objects are constexpr
    static_assert(subMat==answer, MSG);

    ASSERT_TRUE(subMat==answer);
}

TEST(matrix, static_constexpr_subMatrixSameSize)
{
    static constexpr int x {1};

    static constexpr Matrix<float, x, x> mat {{{5.0F}}};

    static constexpr Matrix<float, x, x> subMat {mat.sub<0,0,0,0>()};

    static constexpr Matrix<float, x, x> answer {{{5.0F}}};

    // Check that created objects are constexpr
    static_assert(subMat==answer, MSG);

    ASSERT_TRUE(subMat==answer);
}

TEST(matrix, static_constexpr_set_row)
{
    static constexpr int x {3};

    static constexpr Matrix<int, 1, x> mat0
    {{{{5, -4, 2}}}};
    static constexpr Matrix<int, 1, x> mat1
    {{{{-1, 2 , 3}}}};
    static constexpr Matrix<int, 1, x> mat2
    {{{{-2, 1, 0}}}};

    static constexpr Matrix<int, 1, x> collection[3] =
    {
        mat0, mat1, mat2
    };

    static constexpr auto mat {setRowsTest<int,x,x>(collection, 0)};

    static constexpr Matrix<int, x, x> answer
    {{{{5, -4, 2}, {-1, 2 , 3}, {-2, 1, 0}}}};

    // Check that created objects are constexpr
    static_assert(mat==answer, MSG);

    ASSERT_TRUE(mat==answer);
}

TEST(matrix, static_constexpr_set_col)
{
    static constexpr int x {3};

    static constexpr Matrix<int, x, 1> mat0
    {{{ {5}, {-4}, {2} }}};
    static constexpr Matrix<int, x, 1> mat1
    {{{ {-1}, {-2}, {3} }}};
    static constexpr Matrix<int, x, 1> mat2
    {{{ {-2}, {1}, {0} }}};

    static constexpr Matrix<int, x, 1> collection[3] =
    {
        mat0, mat1, mat2
    };

    static constexpr auto mat {setColsTest<int,x,x>(collection, 0)};

    static constexpr Matrix<int, x, x> answer
    {{{ {5, -1, -2}, {-4, -2 , 1}, {2, 3, 0} }}};

    // Check that created objects are constexpr
    static_assert(mat==answer, MSG);

    ASSERT_TRUE(mat==answer);
}

TEST(matrix, static_constexpr_set_sub_row)
{
    static constexpr size_t x {3};
    static constexpr size_t start {1};
    static constexpr size_t end {2};

    static constexpr Matrix<int, x, x> original
    {{{ {5, -1, -2}, {-4, -2 , 1}, {2, 3, 0} }}};

    static constexpr Matrix<int,1,2> sub
    {{{ {99, -283} }}};

    static constexpr Matrix<int, x, x> answer
    {{{ {5, -1, -2}, {-4, -2 , 1}, {2, 99, -283} }}};

    static constexpr Matrix<int,x,x> mat {setSubRowTest<int,x,x,start,end>(original, sub, 2)};

    // Check that created objects are constexpr
    static_assert(mat==answer, MSG);

    ASSERT_TRUE(mat==answer);
}

TEST(matrix, static_constexpr_set_sub_col)
{
    static constexpr size_t x {3};
    static constexpr size_t start {1};
    static constexpr size_t end {2};

    static constexpr Matrix<int, x, x> original
    {{{ {5, -1, -2}, {-4, -2 , 1}, {2, 3, 0} }}};

    static constexpr Matrix<int,2,1> sub
    {{{ {99}, {-283} }}};

    static constexpr Matrix<int, x, x> answer
    {{{ {5, -1, -2}, {-4, 99, 1}, {2, -283, 0} }}};

    static constexpr Matrix<int,x,x> mat {setSubColTest<int,x,x,start,end>(original, sub, 1)};

    // Check that created objects are constexpr
    static_assert(mat==answer, MSG);

    ASSERT_TRUE(mat==answer);
}

TEST(matrix, static_constexpr_set_sub_mat_square)
{
    static constexpr size_t x {3};

    static constexpr size_t x1 {1};
    static constexpr size_t x2 {2};
    static constexpr size_t y1 {1};
    static constexpr size_t y2 {2};

    static constexpr Matrix<int, x, x> original
    {{{ {5, -1, -2}, {-4, -2 , 1}, {2, 3, 0} }}};

    static constexpr Matrix<int, x-1, x-1> sub
    {{{ {381 , -39}, {33, 15} }}};

    static constexpr Matrix<int, x, x> answer
    {{{ {5, -1, -2}, {-4, 381, -39}, {2, 33, 15} }}};

    static constexpr Matrix<int,x,x> mat {setSubTest<int,x,x,x1,y1,x2,y2>(original, sub)};

    // Check that created objects are constexpr
    static_assert(mat==answer, MSG);

    ASSERT_TRUE(mat==answer);
}

TEST(matrix, static_constexpr_test_square)
{
    static constexpr size_t x {3};
    static constexpr size_t s {2};

    static constexpr Matrix<int, x, x> square
    {{{ {5, -1, -2}, {-4, -2 , 1}, {2, 3, 0} }}};

    static constexpr Matrix<int, x, s> notSquare
    {{{ {381 , -39}, {33, 15}, {72, 99} }}};

    static constexpr bool checkSquare {square.isSquare()};
    static constexpr bool checkNotSquare {notSquare.isSquare()};

    // Check that created objects are constexpr
    static_assert(checkSquare==true, MSG);
    static_assert(checkNotSquare==false, MSG);

    ASSERT_TRUE(checkSquare);
    ASSERT_FALSE(checkNotSquare);
}

TEST(matrix, check_symmetry)
{
    static constexpr size_t x {3};
    static constexpr size_t s {5};

    static constexpr consteig::Matrix<int, s, s> symmetricIntMat
    {{{
        {-5, -4,   2,   1,  77},
        {-4,  5,   7,   8,  9 },
        { 2,  7,   0, -83,  2 },
        { 1,  8, -83,   3,  4 },
        { 77, 9,   2,   4,  2 },
    }}};

    static constexpr consteig::Matrix<int, s, s> asymmetricIntMat
    {{{
        {-5, -4,   2,   1,  77},
        {-4,  5,   7,   8,  9 },
        { 2,  7,   0, -83,  2 },
        { 1,  8, -83,   3,  9 },
        { 77, 9,   2,   4,  2 },
    }}};

    static constexpr bool checkSymmetry {symmetricIntMat.isSymmetric()};
    static constexpr bool checkAsymmetry {asymmetricIntMat.isSymmetric()};

    // Check that created objects are constexpr
    static_assert(checkSymmetry==true, MSG);
    static_assert(checkAsymmetry==false, MSG);

    ASSERT_TRUE(checkSymmetry);
    ASSERT_FALSE(checkAsymmetry);
}
