![example workflow](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg?branch=main)
[![Documentation](https://img.shields.io/badge/docs-online-blue)](https://mitchellthompkins.github.io/consteig/)

# consteig

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
* Optional [gcem](https://github.com/MitchellThompkins/gcem) as an alternative constexpr math backend.

Full documentation including guidance, constraints, numerical methodology,
examples, verification, and performance are described in the
[documentation](https://mitchellthompkins.github.io/consteig/).

# Usage

Simply `#include <consteig/consteig.hpp>` (optionally consume via CMake with
`add_subdirectory`, `FetchContent`, vcpkg, or Conan). Requires C++17.

For a quick reference on getting started, including examples, see
[getting-started](https://mitchellthompkins.github.io/consteig/getting-started/).
