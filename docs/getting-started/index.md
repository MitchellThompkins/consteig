title: Getting Started

# Getting Started

## Installation

consteig is header-only. Copy `consteig.hpp` and the rest of the headers into your project (or add the repository as a submodule), then:

```cpp
#include "consteig.hpp"
```

No separate compilation step is needed. The CMake files in the repository are for testing and development only.

**Requirement**: A C++17-capable compiler (GCC 7+, Clang 5+, MSVC 19.14+).

## Your First Eigenvalue Computation

```cpp
#include "consteig.hpp"

// Define a matrix — all values must be known at compile time
static constexpr consteig::Matrix<double, 2, 2> A{{
    {3.0, 1.0},
    {1.0, 3.0}
}};

// Compute eigenvalues at compile time
static constexpr auto eigs = consteig::eigvals(A);

// eigs is a Matrix<Complex<double>, 2, 1>
// eigs(0,0).real ≈ 4.0,  eigs(0,0).imag ≈ 0.0
// eigs(1,0).real ≈ 2.0,  eigs(1,0).imag ≈ 0.0
```

## Verifying Results with static_assert

The most powerful pattern is to verify results at compile time. If the assertion fails, the build fails — no runtime needed:

```cpp
#include "consteig.hpp"
#include "test_dependencies/test_tools.hpp"  // for CONSTEIG_TEST_TOLERANCE

static constexpr consteig::Matrix<double, 2, 2> A{{
    {3.0, 1.0},
    {1.0, 3.0}
}};

static constexpr auto eigs = consteig::eigvals(A);

// checkEigenValues verifies trace and determinant invariants
static_assert(consteig::checkEigenValues(A, eigs, 1e-9),
              "Eigenvalue verification failed");
```

## Computing Eigenvectors

```cpp
static constexpr auto eigs = consteig::eigvals(A);
static constexpr auto vecs = consteig::eigvecs(A, eigs);

// vecs is a Matrix<Complex<double>, N, N>
// Column i of vecs is the eigenvector for eigs(i, 0)
```

## Declaring Matrices

Matrix dimensions are template parameters. Values are provided as nested initializer lists:

```cpp
// 3x3 matrix
static constexpr consteig::Matrix<double, 3, 3> M{{
    {1.0, 2.0, 3.0},
    {4.0, 5.0, 6.0},
    {7.0, 8.0, 9.0}
}};

// Access element at row 1, column 2 (zero-based)
static constexpr double val = M(1, 2);  // 6.0
```

!!! note "Initializer syntax"
    The double braces `{{...}}` are required: the outer pair initializes the `Matrix`, the inner pairs initialize each row. See [Matrix Operations](../guide/matrix.md) for more detail.

## Next Steps

- [Matrix Operations](../guide/matrix.md) — construction, arithmetic, slicing
- [Eigensolvers](../guide/eigensolvers.md) — `eigvals`, `eigvecs`, verification
- [Examples](../examples/population.md) — real-world use cases
- [Configuration](../guide/configuration.md) — tuning for large or difficult matrices
