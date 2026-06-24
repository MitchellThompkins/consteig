---
title: Matrix Operations
---

# Matrix Operations

## Declaring a Matrix

`Matrix<T, R, C>` is a fixed-size matrix with scalar type `T`, `R` rows, and `C` columns. All dimensions are compile-time constants.

```cpp
// 3x3 double matrix
static constexpr consteig::Matrix<double, 3, 3> A{{
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0},
    {7.0, 8.0, 9.0}
}};

// 3x3 double matrix
static constexpr consteig::Matrix<double, 3, 3> B{{
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0},
    {7.0, 8.0, 9.0}
}};
```

The double-brace syntax (`{{ }}`) is required: the outer `{}` initializes the `Matrix`, and the inner `{}` initializes each row.

## Element Access

Elements are accessed with `operator()(row, col)` using zero-based indices:

```cpp
static constexpr double val = A(0, 1);  // row 0, col 1 = 2.0
```

## Arithmetic

Standard matrix arithmetic is supported between matrices of compatible dimensions:

```cpp
static constexpr auto C = A + B;   // element-wise addition
static constexpr auto D = A - B;   // element-wise subtraction
static constexpr auto E = A * B;   // matrix multiplication (dimensions must be compatible)
static constexpr auto F = 2.0 * A; // scalar multiplication
```

Matrix multiplication requires the column count of the left operand to equal the row count of the right operand, enforced by `static_assert` at compile time.

## Transpose

```cpp
static constexpr auto At = consteig::transpose(A);
// Or via member function:
static constexpr auto At2 = A.transpose();
```

## Identity and Diagonal Matrices

```cpp
static constexpr auto I3 = consteig::eye<double, 3>();          // 3x3 identity
static constexpr auto D3 = consteig::diagonal<double, 3>(5.0);  // 5 on diagonal
```

## Norms

```cpp
static constexpr double frob = consteig::norm(A);     // Frobenius norm
static constexpr double one  = consteig::norm1(A);    // 1-norm (max column sum)
static constexpr double inf  = consteig::normInf(A);  // inf-norm (max row sum)
// Or via member function:
static constexpr double frob2 = A.norm();             // same as consteig::norm(A)
```

## Dot Product

For 1xN row vectors:

```cpp
static constexpr consteig::Matrix<double, 1, 3> u{{{1.0, 2.0, 3.0}}};
static constexpr consteig::Matrix<double, 1, 3> v{{{4.0, 5.0, 6.0}}};
static constexpr double d = consteig::dot(u, v);  // 32.0
```

## Determinant and Trace

```cpp
static constexpr double d = consteig::determinant(A);  // Laplace expansion, O(n!), small matrices only
static constexpr double t = consteig::trace(A);        // sum of diagonal elements
// Or via member functions:
static constexpr double d2 = A.determinant();
static constexpr double t2 = A.trace();
```

**`determinant()` uses Laplace expansion with O(n!) complexity. It is practical
only for small matrices (n <= 4 or 5). For larger matrices it is used only
internally for eigenvalue verification.**

## Extracting Rows, Columns, and Submatrices

```cpp
// Full row/column
static constexpr auto row0 = A.row(0);          // 1x3 matrix
static constexpr auto col1 = A.col(1);          // 3x1 matrix

// Partial row/column (compile-time bounds)
static constexpr auto partial = A.row<0, 1>(0); // columns 0..1 of row 0

// Rectangular submatrix: compile-time size, runtime start position
// block<numRows, numCols>(startRow, startCol)
static constexpr auto sub = A.template block<2, 2>(0, 1); // 2x2 block starting at (0,1)
```

## Setting Rows, Columns, and Submatrices

```cpp
consteig::Matrix<double, 3, 3> M{};

static constexpr consteig::Matrix<double, 1, 3> newRow{{{9.0, 8.0, 7.0}}};
M.setRow(newRow, 1);  // overwrite row 1

static constexpr consteig::Matrix<double, 3, 1> newCol{{{1.0}, {2.0}, {3.0}}};
M.setCol(newCol, 2);  // overwrite column 2

// Overwrite a rectangular subregion: compile-time size, runtime start
static constexpr consteig::Matrix<double, 2, 2> patch{{{1.0, 2.0}, {3.0, 4.0}}};
M.template setBlock<2, 2>(patch, 0, 1);  // write patch starting at (0,1)
```

## Symmetry Check

```cpp
static constexpr bool sym = A.isSymmetric();               // uses machine epsilon
static constexpr bool sym2 = A.isSymmetric(1e-6);          // explicit tolerance
static constexpr bool sq = A.isSquare();
```

## Characteristic Polynomial

`char_poly(A)` computes the monic characteristic polynomial of a square matrix
using the Faddeev-LeVerrier algorithm [^1]. It returns a column vector of N+1
coefficients in descending power order, where the polynomial is:

```
det(lam*I - A) = lam^N + c_1*lam^(N-1) + ... + c_N
```

```cpp
static constexpr consteig::Matrix<double, 3, 3> A{{
    {1.0, 0.0, 0.0},
    {0.0, 2.0, 0.0},
    {0.0, 0.0, 3.0}
}};

// coeffs(0,0) = 1, coeffs(1,0) = -6, coeffs(2,0) = 11, coeffs(3,0) = -6
// represents: lam^3 - 6*lam^2 + 11*lam - 6 = (lam-1)(lam-2)(lam-3)
static constexpr auto coeffs = consteig::char_poly(A);
```

The algorithm uses only matrix multiplications and traces, with no eigenvalue
computation or complex arithmetic. Note that it is susceptible to catastrophic
cancellation for matrices with near-repeated eigenvalues.

## Element-Wise Square Root

```cpp
static constexpr auto sqrtA = consteig::sqrt(A);  // applies sqrt to each element
```

[^1]: Wilson, J. (2024). [Derivation of the Faddeev-LeVerrier algorithm](https://jollywatt.github.io/notes/flv-derivation.pdf).
