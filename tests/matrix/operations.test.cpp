#include <gtest/gtest.h>

#include "operations.hpp"
#include "test_tools.hpp"

using namespace consteig;

TEST(matrix, static_constexpr_addition)
{
    static constexpr int s{2};
    static constexpr Matrix<int, s, s> mat1{{{3, 4}, {5, 6}}};
    static constexpr Matrix<int, s, s> mat2{{{1, 3}, {2, 5}}};

    static constexpr Matrix<int, s, s> answer{{{4, 7}, {7, 11}}};

    static constexpr Matrix<int, s, s> test{mat1 + mat2};

    // Check that created objects are constexpr
    static_assert(answer == test, MSG);

    ASSERT_TRUE(answer == test);
}

TEST(matrix, regular_addition)
{
    static constexpr int r{3};
    static constexpr int c{2};

    Matrix<int, r, c> mat1 = {{{3, 4}, {5, 6}, {3, 9}}};
    Matrix<int, r, c> mat2 = {{{1, 3}, {2, 5}, {1, 10}}};

    static constexpr Matrix<int, r, c> addAnswer{{{4, 7}, {7, 11}, {4, 19}}};
    static constexpr Matrix<int, r, c> subAnswer{{{2, 1}, {3, 1}, {2, -1}}};

    Matrix<int, r, c> add{mat1 + mat2};
    Matrix<int, r, c> sub{mat1 - mat2};

    ASSERT_TRUE(add == addAnswer);
    ASSERT_TRUE(sub == subAnswer);
}

TEST(matrix, static_constexpr_scalar)
{
    static constexpr int r{3};
    static constexpr int c{2};

    static constexpr int scalar{2};
    static constexpr Matrix<int, r, c> mat{{{3, 4}, {5, 6}, {3, 9}}};

    static constexpr Matrix<int, r, c> answer{{{6, 8}, {10, 12}, {6, 18}}};

    static constexpr Matrix<int, r, c> mult{scalar * mat};

    // Check that created objects are constexpr
    static_assert(mult == answer, MSG);

    ASSERT_TRUE(mult == answer);
}

TEST(matrix, static_constexpr_multiplication)
{
    static constexpr int r{3};
    static constexpr int c{2};

    static constexpr Matrix<int, r, c> mat1{{{3, 4}, {5, 6}, {3, 9}}};
    static constexpr Matrix<int, c, r> mat2{{{1, -3, 7}, {2, 5, 9}}};
    static constexpr Matrix<int, c, c> mat3{{{0, 1}, {2, 3}}};

    static constexpr Matrix<int, r, r> answer1{
        {{11, 11, 57}, {17, 15, 89}, {21, 36, 102}}};
    static constexpr Matrix<int, c, c> answer2{{{9, 49}, {58, 119}}};
    static constexpr Matrix<int, c, c> answer3{{{2, 3}, {6, 11}}};

    static constexpr Matrix<int, r, r> mult1{mat1 * mat2};
    static constexpr Matrix<int, c, c> mult2{mat2 * mat1};
    static constexpr Matrix<int, c, c> mult3{mat3 * mat3};

    // Check that created objects are constexpr
    static_assert(mult1 == answer1, MSG);
    static_assert(mult2 == answer2, MSG);
    static_assert(mult3 == answer3, MSG);

    ASSERT_TRUE(mult1 == answer1);
    ASSERT_TRUE(mult2 == answer2);
    ASSERT_TRUE(mult3 == answer3);
}

TEST(matrix, static_constexpr_trans)
{
    static constexpr int r{3};
    static constexpr int c{2};

    static constexpr Matrix<int, r, c> mat1{{{3, 4}, {5, 6}, {3, 9}}};
    static constexpr Matrix<int, r, r> mat2{{{3, 4, 8}, {5, 6, 7}, {3, 9, 8}}};

    static constexpr Matrix<int, c, r> answer1{{{3, 5, 3}, {4, 6, 9}}};
    static constexpr Matrix<int, r, r> answer2{
        {{3, 5, 3}, {4, 6, 9}, {8, 7, 8}}};

    static constexpr Matrix<int, c, r> mat1Transpose{transpose(mat1)};
    static constexpr Matrix<int, r, r> mat2Transpose{transpose(mat2)};

    // Check that created objects are constexpr
    static_assert(mat1Transpose == answer1, MSG);
    static_assert(mat2Transpose == answer2, MSG);

    ASSERT_TRUE(mat1Transpose == answer1);
    ASSERT_TRUE(mat2Transpose == answer2);
}

TEST(matrix, static_constexpr_dot)
{
    static constexpr int n{4};

    static constexpr Matrix<int, 1, n> mat1{{{3, 4, 4, 8}}};
    static constexpr Matrix<int, 1, n> mat2{{{3, 5, 3, 9}}};
    static constexpr Matrix<int, n, 1> mat3{{{3}, {5}, {3}, {9}}};

    static constexpr int dotProduct1{dot(mat1, mat2)};
    static constexpr int dotProduct2{dot(transpose(mat3), mat1)};
    static constexpr int answer{113};

    // Check that created objects are constexpr
    static_assert(dotProduct1 == answer, MSG);
    static_assert(dotProduct2 == answer, MSG);

    ASSERT_TRUE(dotProduct1 == answer);
    ASSERT_TRUE(dotProduct2 == answer);
}

TEST(matrix, static_constexpr_diag)
{
    static constexpr int x{3};

    static constexpr Matrix<float, x, x> answer{
        {{1.0F, 0, 0}, {0, 1.0F, 0}, {0, 0, 1.0F}}};

    static constexpr auto mat{diagonal<float, x>(1.0F)};

    // Check that created objects are constexpr
    static_assert(mat == answer, MSG);

    ASSERT_TRUE(mat == answer);
}

TEST(matrix, static_constexpr_norm_euclidean)
{
    static constexpr int x{3};
    static constexpr int r{2};

    static constexpr Matrix<float, x, x> mat1{
        {{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F, 3.0F}, {-2.0F, 1.0F, 0.0F}}};

    static constexpr Matrix<float, r, r> mat2{{{1.0F, -7.0F}, {-2.0F, 3.0F}}};

    static constexpr float n1{norm(mat1)};
    static constexpr float n2{norm(mat2)};

    static constexpr float answer1 = 8.0F;
    static constexpr float answer2 = 7.937253933F;

    // ASSERT_FLOAT_EQ will fail in this case, as answer2 above was rounded
    ASSERT_NEAR(n1, answer1, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_NEAR(n2, answer2, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, static_constexpr_det)
{
    static constexpr int x{3};
    static constexpr int r{2};

    static constexpr Matrix<float, x, x> mat1{
        {{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F, 3.0F}, {-2.0F, 1.0F, 0.0F}}};

    static constexpr Matrix<float, r, r> mat2{{{1.0F, -7.0F}, {-2.0F, 3.0F}}};

    static constexpr float n1{determinant(mat1)};
    static constexpr float n2{determinant(mat2)};

    static constexpr float answer1 = 15.0F;
    static constexpr float answer2 = -11.0F;

    static_assert(n1 == answer1, MSG);
    static_assert(n2 == answer2, MSG);

    // ASSER_FLOAT_EQ will fail in this case, as answer2 above was rounded
    ASSERT_NEAR(n1, answer1, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_NEAR(n2, answer2, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, static_constexpr_sqrt)
{
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{4.0F, 9.0F}, {16.0F, 25.0F}}};
    static constexpr Matrix<float, s, s> answer{{{2.0F, 3.0F}, {4.0F, 5.0F}}};

    static constexpr Matrix<float, s, s> result{sqrt(mat)};

    static_assert(result == answer, MSG);
    ASSERT_TRUE(result == answer);
}

// ---- Member wrapper tests ----

TEST(matrix, member_transpose)
{
    static constexpr int r{3};
    static constexpr int c{2};

    static constexpr Matrix<int, r, c> mat1{{{3, 4}, {5, 6}, {3, 9}}};
    static constexpr Matrix<int, r, r> mat2{{{3, 4, 8}, {5, 6, 7}, {3, 9, 8}}};

    static constexpr Matrix<int, c, r> answer1{{{3, 5, 3}, {4, 6, 9}}};
    static constexpr Matrix<int, r, r> answer2{
        {{3, 5, 3}, {4, 6, 9}, {8, 7, 8}}};

    static constexpr Matrix<int, c, r> t1{mat1.transpose()};
    static constexpr Matrix<int, r, r> t2{mat2.transpose()};

    static_assert(t1 == answer1, MSG);
    static_assert(t2 == answer2, MSG);

    ASSERT_TRUE(t1 == answer1);
    ASSERT_TRUE(t2 == answer2);
}

TEST(matrix, member_trace)
{
    static constexpr int s{3};

    static constexpr Matrix<int, s, s> mat{{{3, 4, 8}, {5, 6, 7}, {3, 9, 8}}};

    static constexpr int t{mat.trace()};
    static constexpr int answer{17};

    static_assert(t == answer, MSG);
    ASSERT_EQ(t, answer);
}

TEST(matrix, member_determinant)
{
    static constexpr int x{3};
    static constexpr int r{2};

    static constexpr Matrix<float, x, x> mat1{
        {{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F, 3.0F}, {-2.0F, 1.0F, 0.0F}}};

    static constexpr Matrix<float, r, r> mat2{{{1.0F, -7.0F}, {-2.0F, 3.0F}}};

    static constexpr float d1{mat1.determinant()};
    static constexpr float d2{mat2.determinant()};

    static_assert(d1 == 15.0F, MSG);
    static_assert(d2 == -11.0F, MSG);

    ASSERT_NEAR(d1, 15.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_NEAR(d2, -11.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, member_norm)
{
    static constexpr int x{3};
    static constexpr int r{2};

    static constexpr Matrix<float, x, x> mat1{
        {{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F, 3.0F}, {-2.0F, 1.0F, 0.0F}}};

    static constexpr Matrix<float, r, r> mat2{{{1.0F, -7.0F}, {-2.0F, 3.0F}}};

    static constexpr float n1{mat1.norm()};
    static constexpr float n2{mat2.norm()};

    ASSERT_NEAR(n1, 8.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_NEAR(n2, 7.937253933F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, member_dot)
{
    static constexpr int n{4};

    static constexpr Matrix<int, 1, n> mat1{{{3, 4, 4, 8}}};
    static constexpr Matrix<int, 1, n> mat2{{{3, 5, 3, 9}}};

    static constexpr int d{mat1.dot(mat2)};
    static constexpr int answer{113};

    static_assert(d == answer, MSG);
    ASSERT_EQ(d, answer);
}

// ---------------------------------------------------------------------------
// norm1 and normInf — these functions now use unqualified abs() dispatch from
// math_backend.hpp. Verify correctness including negative-valued matrices.
// ---------------------------------------------------------------------------

TEST(matrix, norm1_positive_matrix)
{
    // 1-norm = max column sum of absolute values
    // mat = [[1, 2], [3, 4]]: col sums = 4, 6 -> norm1 = 6
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{1.0F, 2.0F}, {3.0F, 4.0F}}};
    static constexpr float n{norm1(mat)};
    static_assert(equalWithin(n, 6.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 6.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, norm1_negative_values)
{
    // Columns with negative entries: abs is required
    // mat = [[-1, -2], [-3, -4]]: col sums = 4, 6 -> norm1 = 6
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{-1.0F, -2.0F}, {-3.0F, -4.0F}}};
    static constexpr float n{norm1(mat)};
    static_assert(equalWithin(n, 6.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 6.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, norm1_mixed_sign_matrix)
{
    // mat = [[5, -4], [-1, 2]]: col1 abs sum = 6, col2 abs sum = 6 -> norm1 = 6
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{5.0F, -4.0F}, {-1.0F, 2.0F}}};
    static constexpr float n{norm1(mat)};
    static_assert(equalWithin(n, 6.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 6.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, norm1_identity)
{
    // For an identity matrix, each column has sum 1 -> norm1 = 1
    static constexpr int s{3};
    static constexpr Matrix<float, s, s> mat{
        {{1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}}};
    static constexpr float n{norm1(mat)};
    static_assert(equalWithin(n, 1.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 1.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, norm1_constexpr_double)
{
    // Check constexpr evaluation for double type
    // mat = [[3, 0], [0, 4]]: col sums = 3, 4 -> norm1 = 4
    static constexpr int s{2};
    static constexpr Matrix<double, s, s> mat{{{3.0, 0.0}, {0.0, 4.0}}};
    static constexpr double n{norm1(mat)};
    static_assert(n == 4.0, MSG);
    ASSERT_EQ(n, 4.0);
}

TEST(matrix, normInf_positive_matrix)
{
    // Inf-norm = max row sum of absolute values
    // mat = [[1, 2], [3, 4]]: row sums = 3, 7 -> normInf = 7
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{1.0F, 2.0F}, {3.0F, 4.0F}}};
    static constexpr float n{normInf(mat)};
    static_assert(equalWithin(n, 7.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 7.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, normInf_negative_values)
{
    // abs is required for correct row sums
    // mat = [[-1, -2], [-3, -4]]: row sums = 3, 7 -> normInf = 7
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{-1.0F, -2.0F}, {-3.0F, -4.0F}}};
    static constexpr float n{normInf(mat)};
    static_assert(equalWithin(n, 7.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 7.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, normInf_mixed_sign_matrix)
{
    // mat = [[5, -4, 2], [-1, 2, 3], [-2, 1, 0]]
    // row abs sums: 11, 6, 3 -> normInf = 11
    static constexpr int x{3};
    static constexpr Matrix<float, x, x> mat{
        {{5.0F, -4.0F, 2.0F}, {-1.0F, 2.0F, 3.0F}, {-2.0F, 1.0F, 0.0F}}};
    static constexpr float n{normInf(mat)};
    static_assert(equalWithin(n, 11.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 11.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, normInf_identity)
{
    // For an identity matrix, each row has sum 1 -> normInf = 1
    static constexpr int s{3};
    static constexpr Matrix<float, s, s> mat{
        {{1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}}};
    static constexpr float n{normInf(mat)};
    static_assert(equalWithin(n, 1.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n, 1.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}

TEST(matrix, normInf_constexpr_double)
{
    // mat = [[3, 0], [0, 4]]: row sums = 3, 4 -> normInf = 4
    static constexpr int s{2};
    static constexpr Matrix<double, s, s> mat{{{3.0, 0.0}, {0.0, 4.0}}};
    static constexpr double n{normInf(mat)};
    static_assert(n == 4.0, MSG);
    ASSERT_EQ(n, 4.0);
}

TEST(matrix, norm1_vs_normInf_differ)
{
    // Confirm that norm1 and normInf are not equivalent in general
    // mat = [[1, 3], [2, 0]]: col sums = 3, 3 -> norm1 = 3; row sums = 4, 2 -> normInf = 4
    static constexpr int s{2};
    static constexpr Matrix<float, s, s> mat{{{1.0F, 3.0F}, {2.0F, 0.0F}}};
    static constexpr float n1{norm1(mat)};
    static constexpr float nInf{normInf(mat)};
    static_assert(equalWithin(n1, 3.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    static_assert(equalWithin(nInf, 4.0F, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
    ASSERT_NEAR(n1, 3.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
    ASSERT_NEAR(nInf, 4.0F, CONSTEIG_FLOAT_TEST_TOLERANCE);
}
