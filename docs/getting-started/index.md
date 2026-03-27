---
title: Getting Started
---

# Getting Started

## Installation

As consteig is a templated library a user does not need to compile anything
separately. Simply `#include "consteig.hpp"` into the project (optionally
consume it via CMake with `add_subdirectory` or `FetchContent`).

consteig also requires a C++ compiler which supports C++17.

Copy `consteig.hpp` and the rest of the headers into your project (or add the
repository as a submodule). No separate compilation step is needed. The CMake
files in the repository are for testing and development only.

Quick reference examples:

* [Working with matrices](https://github.com/mitchellthompkins/consteig/blob/develop/examples/matrix.cpp)
* [Finding eigenvalues](https://github.com/mitchellthompkins/consteig/blob/develop/examples/eigen.cpp)
* [Population flow](https://github.com/mitchellthompkins/consteig/blob/develop/examples/population.cpp)
* [Butterworth filter design](https://github.com/mitchellthompkins/consteig/blob/develop/examples/butterworth/butterworth_core.hpp)
* [DC motor control gain validation](https://github.com/mitchellthompkins/consteig/blob/develop/examples/dc_motor_control.cpp)

## Your First Eigenvalue Computation

```cpp
#include "consteig.hpp"

// Define a matrix — all values must be known at compile time
static constexpr consteig::Matrix<double, 2, 2> A{{
    {3.0, 1.0},
    {1.0, 3.0}
}};

// Compute eigenvalues at compile time
static constexpr auto eigs = consteig::eigenvalues(A);

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

static constexpr auto eigs = consteig::eigenvalues(A);

// checkEigenValues verifies trace and determinant invariants
static_assert(consteig::checkEigenValues(A, eigs, 1e-9),
              "Eigenvalue verification failed");
```

## Computing Eigenvectors

```cpp
static constexpr auto eigs = consteig::eigenvalues(A);
static constexpr auto vecs = consteig::eigenvectors(A, eigs);

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
- [Eigensolvers](../guide/eigensolvers.md) — `eigenvalues`, `eigenvectors`, verification
- [Examples](../examples/population.md) — real-world use cases
- [Configuration](../guide/configuration.md) — tuning for large or difficult matrices

## Development

### With Docker (recommended)

All build dependencies are packaged inside the dev container, and make is used as
a matter of convenience.

```bash
make container.pull
export MY_UID=$(id -u)
export MY_GID=$(id -g)
make container.make.build
make container.make.test
```

Any make target can be run inside the container via `container.make.<target>`.

The container can be dropped into with `make container.start`.

### Without Docker (native)

Required:

* gcc and/or clang (C++17 support)
* cmake (>= 3.13)
* make
* clang-format (for `make format`)

Optional:

* Octave (for regenerating test cases via `make generate-test-cases`)

```bash
make build
make test
```
