---
title: Eigensolvers
---

# Eigensolvers

## eigenvalues: Eigenvalues Only

The primary entry point for most use cases. Returns all eigenvalues as a column vector of `Complex<T>`.

```cpp
static constexpr consteig::Matrix<double, 3, 3> A{{
    { 2.0, -1.0,  0.0},
    {-1.0,  2.0, -1.0},
    { 0.0, -1.0,  2.0}
}};

static constexpr auto eigs = consteig::eigenvalues(A);
// eigs is Matrix<Complex<double>, 3, 1>
// eigs(0,0), eigs(1,0), eigs(2,0) are the three eigenvalues
```

Real eigenvalues have zero imaginary part. Complex eigenvalues appear as conjugate pairs.

## eigenvectors: Eigenvectors

Given a matrix and its eigenvalues, compute the corresponding eigenvectors. Each
column of the result is the eigenvector for the eigenvalue at the same index.

```cpp
static constexpr auto eigs = consteig::eigenvalues(A);
static constexpr auto vecs = consteig::eigenvectors(A, eigs);
// vecs is Matrix<Complex<double>, 3, 3>
// vecs.col(i) is the eigenvector for eigs(i, 0)
```

## EigenSolver: Combined Class

`EigenSolver<T, S>` computes both eigenvalues and eigenvectors in one object. Use it when you need both and want to avoid calling `eigenvalues` twice.

```cpp
static constexpr consteig::EigenSolver<double, 3> solver(A);
static constexpr auto eigs = solver.eigenvalues();  // Matrix<Complex<double>, 3, 1>
static constexpr auto vecs = solver.eigenvectors(); // Matrix<Complex<double>, 3, 3>
```

## eig: Schur Form

`eig()` returns the real Schur form of the matrix (quasi-upper-triangular). The diagonal entries are real eigenvalues; 2x2 diagonal blocks encode complex conjugate pairs. In most cases you want `eigenvalues` or `eigenvectors` instead.

```cpp
static constexpr auto schur = consteig::eig(A);
```

## Verifying Eigenvalues with checkEigenValues

`checkEigenValues` verifies computed eigenvalues against two matrix invariants:

1. The sum of eigenvalues equals the matrix trace.
2. The product of eigenvalues equals the matrix determinant (only checked for matrices up to 4x4).

Use this in a `static_assert` to make the build fail if eigenvalues are wrong:

```cpp
static constexpr auto eigs = consteig::eigenvalues(A);
static_assert(consteig::checkEigenValues(A, eigs, 1e-9),
              "Eigenvalue check failed");
```

## Tolerance Guidelines

| Matrix type | Recommended tolerance |
|---|---|
| Well-conditioned (general) | `1e-9` |
| Iterative methods (Hessenberg, QR) | `3e-4` |
| Defective matrices (Jordan blocks) | `0.03` |
| Large magnitude eigenvalues | `1.0` |

See [Verification & Accuracy](../verification.md) for a detailed discussion.

## Symmetric vs Non-Symmetric Matrices

The solver automatically routes between two algorithms:

- Symmetric matrices: single-shift QR (`eig_shifted_qr`): faster, assumes real eigenvalues
- Non-symmetric matrices: double-shift QR (`eig_double_shifted_qr`): handles complex conjugate pairs

The routing threshold is controlled by `CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE`.
See [Configuration](configuration.md).

## Working with Complex Results

Eigenvalues are returned as `Complex<T>` even when they are real. To extract the real part:

```cpp
static constexpr double real_part = eigs(0, 0).real;
static constexpr double imag_part = eigs(0, 0).imag;
static constexpr double magnitude = consteig::abs(eigs(0, 0));
```

For real eigenvalues, `imag_part` will be zero (or very close to zero within numerical tolerance).
