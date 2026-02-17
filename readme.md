![example workflow](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg)

## What Is This
Consteig is a constexpr template library which uses both constexpr functions and
template meta-programming to calculate the eigenvalues of a square matrix at
compile time. That is, the eigenvalues can be saved as `static constexpr` values
and no processor execution time is spent calculating them at run-time. Consteig
also allows for compile time static matrix manipulation. To remove any external
dependences several constexpr functions are implemented as well.

## What Does This Do
* Compute real and complex eigenvalues at compile-time for both symmetric and non-symmetric matrices.
* Declare, manipulate, perform matrix operations, and a handful of matrix
  decompositions at compile time. That means users can create matrices and
  perform a collection of [operations](matrix/operations.hpp) on them.
* Perform a selection of math functions (including complex arithmetic) at compile time.
* Strictly freestanding: The core library has zero dependencies on the standard library.

An example helps best. Let's say that we take the example from [Using
Eigenvectors to Find Steady State Population Flows](https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764)...TODO

## Why Does This Exist
Originally this library was developed to support a generic digital filter
library targeted at embedded systems in which the digital filter coefficients
could be calculated at compile time. In order to find the filter coefficients
it’s required to solve a multi order polynomial. Implementing a root finder
natively is tedious and error prone. Finding the roots of a polynomial can be
reframed as an eigenvalue problem which makes life easier. The matlab and octave
root finding functions actually operate in this way. As such development began
on a compile time eigenvalue finder.

## How To Use Consteig
Consteig is a templated library and as such a user does not need to compile
anything separately. Simply `#include “consteig.hpp”` into your project. The
cmake files here are only to facilitate testing and development.

You will also need a C++ compiler which supports **C++17**.

Here are some examples to help get started:
* [Declaring a matrix](examples/matrix.cpp)
* [Matrix Arithmetic](examples/matrix.cpp)
* [Finding eigenvalues](examples/eigen.cpp)
* [Population flow](examples/population.cpp)

## How Is This Different
There are powerful open source C++ eigenvalues solvers already in existence
which are more robust, better optimized, and better tested. The caveat is that
they cannot currently calculate Eigenvalues at compile time.

Another key component to most eigenvalues solvers is the reliance on the
standard library. This is for good reason as the standard library, in particular
the STL containers, are powerful and solve many commonly faced problems.
Unfortunately in some systems, particularly for embedded systems the standard
library isn’t available.

This solves those two problems in a limited capacity.

## When To Use Consteig
* Eigenvalues (real or complex) need to be known at compile time.
* Eigenvalues need to be known and the standard library is unavailable.
* You need to manipulate static matrices at compile time.

## When Not To Use Consteig
* Eigen Values do not need to be known at compile time.
* Eigen Values can be solved at runtime and the compiler can leverage standard
  library functionality.
* Matrices do not need to be manipulated at compile time.

## Verification
The library is rigorously verified through two primary methods:
1. **Eigen Library Comparison**: Unit tests link against the [Eigen](https://eigen.tuxfamily.org/) library to compare compile-time results against a high-performance reference implementation.
2. **Octave Test Generation**: An Octave script (`octave/generate_test_cases.m`) is provided to generate fresh matrix test data and expected results, which are automatically verified using `static_assert` at compile time.

## What Can Improve
* Declaring matrices can be initializer bracket hell. Refer to [this example](examples/matrix.cpp)
  for help.
* The algorithms implemented here do not necessarily take advantage of a
  collection of optimization techniques for solving matrices nor has there been
  a concerted effort to optimize the implementations themselves. As such, for
  extremely large matrices compilation may be slow. 
* Currently the matrix [decompositions](examples/decomp.cpp) require square matrices.
* Support for non-square QR decomposition and optimized determinant/inverse calculations.

## Notes On Performance
* Unit testing _does_ leverage components of the standard library and Eigen, but
  the `consteig` library core does not.

## Building
Build dependencies rely on:
* gcc/g++ (C++17 support)
* cmake (>= 3.11)
* make
* Octave (optional, for test generation)

These are all packaged into an alpine docker container. 
```
make container.pull
export MY_UID=$(id -u)
export MY_GID=$(id -g)
make container.start
```

Then:
```
make build
make test
```

## TODO
- [ ] Compile with `-wall`
- [ ] Optimize QR decomposition using scaled Householder reflections (currently uses Gram-Schmidt for constexpr stability)

## References

## Thanks
Development began by leveraging [gcem](https://github.com/kthohr/gcem) for all
ofthe constexpr math. However, as it using the standard library, it's usage was
temporary. That constexpr math implementations are inspired from the gcem
implementations.
