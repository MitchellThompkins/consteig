---
title: consteig
---

# consteig

![CI](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg?branch=main)

consteig is a header-only C++17 constexpr template library for computing
eigenvalues and eigenvectors of constexpr matrices at compile-time. The results
are stored as `static constexpr` values, so no processor time is spent
calculating them at runtime and no offline tool is needed to generate them.

This is particularly useful when a system's parameters are fixed at
compile-time. Information like filter coefficients, system eigenvalues, and
steady-state distributions can be computed by the compiler and live directly in
the program binary rather than being derived externally and hard-coded. If
parameters change, the compiler recomputes the dependent outputs. When the math
is wrong, `static_assert` can catch it at build time. consteig is strictly
freestanding and depends on no external libraries, not even the C++ standard
library.

All at compile-time, consteig supports:

* Computation of real and complex eigenvalues and eigenvectors for symmetric and
  non-symmetric matrices.
* Matrix construction and manipulation, including common operations and decompositions.
* A selection of mathematical functions, including complex arithmetic.

## Quick Start

```cpp
#include "consteig.hpp"

static constexpr consteig::Matrix<double, 3, 3> M{{
    { 2.0, -1.0,  0.0},
    {-1.0,  2.0, -1.0},
    { 0.0, -1.0,  2.0}
}};

static constexpr auto eigenvalues = consteig::eigenvalues(M);
```

See [Getting Started](getting-started.md) for installation and a full walkthrough.

## When To Use consteig

* Eigenvalues (real or complex) need to be known at compile-time.
* Eigenvalues need to be known and the standard library is unavailable.
* Static matrices need to be manipulated at compile-time.

## Constraints

* Matrix dimensions are template parameters and must be compile-time constants.
* Input matrices must be real-valued. Eigenvalue output may be complex, but
  `Matrix<consteig::Complex<T>, ...>` is not supported as input.
* Only square matrices are supported for eigenvalue computation and
  decompositions.

## Why Does This Exist

This library was originally developed to support a generic digital filter
library for embedded systems, where filter coefficients are calculated at
compile-time. Computing those coefficients requires solving a high-order
polynomial, which can be reframed as an eigenvalue problem, making it far more
manageable. This is the same approach used by
[LAPACK](https://netlib.org/lapack/), and by extension MATLAB and Octave's
root-finding functions.

## How Is This Different

Powerful open-source C++ eigenvalue solvers already exist that are more robust,
optimized, and tested than anything here. However, they share two limitations:
they cannot compute eigenvalues at compile-time, and they depend on the
standard library, which is unavailable on some embedded systems. This library
addresses both constraints.
