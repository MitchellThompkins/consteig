title: consteig

# consteig

![CI](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg)

**Compile-time eigenvalue and eigenvector computation for C++17.**

consteig is a constexpr template library that computes eigenvalues and eigenvectors of square matrices entirely at compile time. Results can be saved as `static constexpr` — no processor time is spent at runtime.

```cpp
#include "consteig.hpp"

// Population transition matrix (known at compile time)
static constexpr consteig::Matrix<double, 2, 2> A{{
    {0.95, 0.20},
    {0.05, 0.80}
}};

// Eigenvalues computed by the compiler, not at runtime
static constexpr auto eigs = consteig::eigenvalues(A);

// Verify at compile time — build fails if wrong
static_assert(consteig::checkEigenValues(A, eigs, 1e-9), "Bad eigenvalues");
```

## Features

- **Eigenvalues and eigenvectors** for real and complex spectra, symmetric and non-symmetric matrices
- **Matrix operations**: arithmetic, norms, transpose, determinant, trace
- **Decompositions**: QR, LU, Hessenberg, Householder
- **Constexpr math functions**: sqrt, exp, pow, trig, complex arithmetic
- **Freestanding core**: no dependency on the C++ standard library
- **Header-only**: just `#include "consteig.hpp"`

## Quick Start

```cpp
#include "consteig.hpp"

static constexpr consteig::Matrix<double, 3, 3> M{{
    { 2.0, -1.0,  0.0},
    {-1.0,  2.0, -1.0},
    { 0.0, -1.0,  2.0}
}};

static constexpr auto eigenvalues = consteig::eigenvalues(M);
// eigenvalues(0,0), (1,0), (2,0) are the three eigenvalues as Complex<double>
```

See [Getting Started](getting-started/index.md) for installation and a full walkthrough.

## When to Use consteig

- Eigenvalues need to be known at compile time (embedded systems, filter design, control theory)
- The standard library is unavailable (freestanding/embedded targets)
- You want to verify system properties (stability, gain margins) at every build via `static_assert`
- You need compile-time matrix manipulation

## Limitations

- Large matrices increase compile time due to the cost of `constexpr` evaluation
- Decompositions require square matrices
- Initializer syntax for matrices is verbose — see [Matrix Operations](guide/matrix.md)
