![example workflow](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg)
[![Documentation](https://img.shields.io/badge/docs-online-blue)](https://mitchellthompkins.github.io/consteig/)

# consteig

consteig is a header-only C++17 constexpr template library for computing
eigenvalues and eigenvectors of constexpr  matrices at compile-time. The results
are stored as `static constexpr` values, so no processor time is spent
calculating them at runtime and no offline tool is needed to generate them.

This is particularly useful when a system's parameters are fixed at
compile-time. Information like filter coefficients, system eigenvalues, and
steady-state distributions can live directly in source code rather than being
derived externally and hard-coded. When parameters change, the compiler
recomputes. When the math is wrong, `static_assert` catches it at build time.
consteig is strictly freestanding and depends on no external libraries, not even
the C++ standard library.

All at compile-time, consteig supports:

* Computation of real and complex eigenvalues and eigenvectors for symmetric and
  non-symmetric matrices.
* Matrix construction and manipulation, including common operations and decompositions.
* A selection of mathematical functions, including complex arithmetic.

# How To Use consteig

Simply `#include "consteig.hpp"` (optionally consume via CMake with
`add_subdirectory` or `FetchContent`). Requires C++17.

Quick reference examples:
* [Working with matrices](examples/matrix.cpp)
* [Finding eigenvalues](examples/eigen.cpp)
* [Population flow](examples/population.cpp)
* [Butterworth filter design](examples/butterworth/butterworth_core.hpp)
* [DC motor control gain validation](examples/dc_motor_control.cpp)

See the [documentation](https://mitchellthompkins.github.io/consteig/) for the
full API reference and configuration options.

# Examples

## Population Flow

If a system's transition matrix is fixed at compile-time, its steady-state
behavior never needs to be recomputed at runtime. The
[`population.cpp`](examples/population.cpp) example computes steady-state
population fractions as `static constexpr` values; at runtime, distributing any
total population is a single multiply with no matrix iteration.

## Digital Filter Design

IIR filter coefficients derived entirely at compile-time from a continuous-time
transfer function. See [`butterworth_core.hpp`](examples/butterworth/butterworth_core.hpp)
and the [documentation](https://mitchellthompkins.github.io/consteig/examples/butterworth/)
for the full walkthrough.

## Control Theory

When system parameters and controller gains are known at compile-time, consteig
can verify that the closed-loop poles meet performance requirements, turning a
runtime failure into a build failure. See [`dc_motor_control.cpp`](examples/dc_motor_control.cpp)
and the [documentation](https://mitchellthompkins.github.io/consteig/examples/dc-motor/)
for the full walkthrough.

![Motor Control](docs/imgs/step_response.png "step_response")


# Why Does This Exist

This library was originally developed to support a generic digital filter
library for embedded systems, where filter coefficients are calculated at
compile-time. Computing those coefficients requires solving a high-order
polynomial, which can be reframed as an eigenvalue problem, making it far more
manageable. This is the same approach used by
[LAPACK](https://netlib.org/lapack/), and by extension MATLAB and Octave's
root-finding functions.

# How Is This Different

Powerful open-source C++ eigenvalue solvers already exist that are more robust,
optimized, and tested than anything here. However, they share two limitations:
they cannot compute eigenvalues at compile-time, and they depend on the
standard library, which is unavailable on some embedded systems. This library
addresses both constraints.

# Algorithmic Approach and Optimizations

See [docs/methods.md](docs/methods.md) for a discussion on the implementation
specifics for the numerical solvers implemented by consteig.

# Verification and Accuracy

See [docs/verification.md](docs/verification.md) for a detailed discussion on
the accuracy and verification methods implemented to test this library.

# Performance

![Compilation Times](profiling/results/compile_times_gcc_15.2.0.png "compilation_times")
![Compilation Memory](profiling/results/compile_times_gcc_15.2.0_memory.png "compilation_memory")

See [profiling/README.md](profiling/README.md) for methodology. The shaded region
indicates where `consteig_raise_compiler_limits` was needed.

# When To Use consteig

* Eigenvalues (real or complex) need to be known at compile-time.
* Eigenvalues need to be known and the standard library is unavailable.
* Static matrices need to be manipulated at compile-time.

# Constraints

* Matrix dimensions are template parameters and must be compile-time constants.
* Input matrices must be real-valued. Eigenvalue output may be complex, but `Matrix<consteig::Complex<T>, ...>` is not supported as input.
* Only square matrices are supported for eigenvalue computation and decompositions.
