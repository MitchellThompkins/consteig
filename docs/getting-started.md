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

* [Working with matrices](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/matrix.cpp)
* [Finding eigenvalues](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/eigen.cpp)
* [DC motor control gain validation](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/dc_motor_control.cpp)
* [Population flow](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/population.cpp)
* [Butterworth filter design](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/butterworth/butterworth_core.hpp)

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
```

## Sanity-Checking the Solver with checkEigenValues

`checkEigenValues` verifies that the computed eigenvalues are self-consistent
with the input matrix by checking two invariants: the sum of eigenvalues equals
the matrix trace, and (for matrices up to 4x4) the product equals the
determinant. This catches solver failures, but does not verify that the
eigenvalues match any user-defined expectation.

```cpp
static constexpr auto eigs = consteig::eigenvalues(A);
static_assert(consteig::checkEigenValues(A, eigs, 1e-9),
              "Eigenvalue invariants failed");
```

To assert something meaningful about your specific problem, write your own
`static_assert` against the computed values directly. For example, to confirm
all poles are stable:

```cpp
static_assert(eigs(0, 0).real < 0.0 && eigs(1, 0).real < 0.0,
              "System is unstable");
```

## Computing Eigenvectors

```cpp
static constexpr auto eigs = consteig::eigenvalues(A);
static constexpr auto vecs = consteig::eigenvectors(A, eigs);

// vecs is a Matrix<Complex<double>, N, N>
// Column i of vecs is the eigenvector for eigs(i, 0)
```

## Declaring Matrices

Matrix dimensions are template parameters. Values are provided as nested
initializer lists (alternatively there is a `consteig::make_matrix` function):

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

The double braces `{{...}}` are required: the outer pair initializes the
`Matrix`, the inner pairs initialize each row. See [Matrix
Operations](../guide/matrix.md) for more detail.

## More Examples

### Control Theory

When system parameters and controller gains are known at compile-time, consteig
can verify that the closed-loop poles meet performance requirements, turning a
runtime failure into a build failure. See the [dc-motor
example](https://mitchellthompkins.github.io/consteig/examples/dc-motor/).

### Population Flow

If a system's transition matrix is fixed at compile-time, its steady-state
behavior never needs to be recomputed at runtime. The
[`population.cpp`](https://github.com/mitchellthompkins/consteig/blob/HEAD/examples/population.cpp) example computes steady-state
population fractions as `static constexpr` values; at runtime, distributing any
total population is a single multiply with no matrix iteration. See the
[population
example](https://mitchellthompkins.github.io/consteig/examples/population/).

### Digital Filter Design

IIR filter coefficients derived entirely at compile-time from a continuous-time
transfer function. See the [butterworth
example](https://mitchellthompkins.github.io/consteig/examples/butterworth/).
