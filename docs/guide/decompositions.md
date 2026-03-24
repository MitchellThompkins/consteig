title: Decompositions

# Decompositions

All decompositions require square matrices and floating-point element types.

## QR Decomposition

Factors A = Q * R where Q is orthogonal and R is upper triangular. Uses Givens rotations for numerical stability.

```cpp
static constexpr consteig::Matrix<double, 3, 3> A{{
    {12.0, -51.0,   4.0},
    { 6.0, 167.0, -68.0},
    {-4.0,  24.0, -41.0}
}};

static constexpr auto result = consteig::qr(A);
// result._q  — orthogonal factor (Q)
// result._r  — upper-triangular factor (R)
// A ≈ result._q * result._r
```

For matrices already in upper Hessenberg form, `qr_hessenberg` is faster and is what the eigensolver uses internally:

```cpp
static constexpr auto result = consteig::qr_hessenberg(H);
```

## Hessenberg Reduction

Reduces a square matrix to upper Hessenberg form via orthogonal similarity: H = P^T * A * P. Hessenberg matrices have zeros below the first subdiagonal.

```cpp
static constexpr auto result = consteig::hess(A);
// result._h  — upper Hessenberg matrix
// result._p  — accumulated orthogonal factor
// A ≈ result._p * result._h * transpose(result._p)
```

Hessenberg reduction is a key preprocessing step for the eigenvalue solver — it reduces the cost of each QR iteration from O(n^3) to O(n^2).

## Householder Reflection

Computes a Householder reflector H = I - 2*v*v^T that zeros the subdiagonal entries of the first column of a matrix. Used internally by Hessenberg reduction.

```cpp
static constexpr auto H = consteig::house(A);
// H is an R×R orthogonal matrix
// H * A.col(0) = alpha * e1  (e1 is the first basis vector)
```

## LU Decomposition

Factors A as PA = LU with partial pivoting, where P is a permutation, L is unit lower triangular, and U is upper triangular.

```cpp
static constexpr consteig::Matrix<double, 3, 3> A{{
    {2.0, 1.0, 1.0},
    {4.0, 3.0, 3.0},
    {8.0, 7.0, 9.0}
}};

static constexpr auto result = consteig::lu(A);
// result._l  — unit lower triangular
// result._u  — upper triangular
// result._p  — pivot index array (not a matrix)
```

## Solving Linear Systems with LU

Use `lu_solve` to solve Ax = b given an LU factorization:

```cpp
static constexpr consteig::Matrix<double, 3, 1> b{{{1.0}, {2.0}, {3.0}}};
static constexpr auto lu_result = consteig::lu(A);
static constexpr auto x = consteig::lu_solve(lu_result, b);
// x satisfies A * x = b
```

`lu_solve` is used internally by the eigenvector solver for inverse iteration but is also available for general use.
