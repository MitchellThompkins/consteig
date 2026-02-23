![example workflow](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg)

## What Is This 
Consteig is a constexpr template library which uses both constexpr functions and
template meta-programming to calculate the eigenvalues of a square matrix at
compile time. That is, the eigenvalues can be saved as `static constexpr` values
and no processor execution time is spent calculating them at run-time. Consteig
also allows for compile time static matrix manipulation. To remove any external
dependences several constexpr math functions are implemented as well.

All at compile time, consteig supports:

* Computation of real and complex eigenvalues for symmetric and non-symmetric matrices.
* Matrix construction and manipulation, including common operations and decompositions.
* A selection of mathematical functions, including complex arithmetic. [^1]
* A strictly freestanding core with no dependence on the C++ standard library.

It's important to note that `consteig` currently focuses exclusively on
computing **eigenvalues**. For many applications, such as determining the
stability of digital filters (as described in "Why Does This Exist"), the
eigenvalues themselves are the desired result, and the eigenvectors are not
required. Therefore, this library does not currently provide functionality for
eigenvector computation.

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
* [Butterworth Filter Design](examples/butterworth_core.hpp)

### Build Options

consteig has a few options which can be modified. However, these defaults are
well tested and modifying them may have non-desirable results such as increased
compile times or numerical instability.

CMake Options:

* `CONSTEIG_RAISE_COMPILER_LIMITS` - A convenience option in the root
  `CMakeLists.txt` that, when enabled (`ON`), automatically applies the
  necessary compiler flags (`-fconstexpr-ops-limit`, `-fconstexpr-steps`, and
  `-fconstexpr-depth`) to allow for complex compile-time calculations. This
  option is only available when building `consteig` directly. If you include
  this library in your project via `add_subdirectory`, you can manually invoke
  the `consteig_raise_compiler_limits()` CMake macro on your targets.

User Macros:

* `CONSTEIG_MAX_ITER` - Controls the maximum number of iterations allowed for
  eigenvalue solvers. Increasing this may help difficult matrices converge, but
  significantly increases compile times and the likelihood of hitting compiler
  step limits.

* `CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE` - A routing threshold used to determine
  if a matrix is "symmetric enough" to use the optimized symmetric eigenvalue
  solver (`eig_shifted_qr`). If the matrix symmetry exceeds this tolerance, the
  library falls back to the more robust but heavier non-symmetric solver
  (`eig_double_shifted_qr`).

* `CONSTEIG_USE_LONG_DOUBLE` - Forces all internal constexpr eigenvalue
  calculations to use `long double`. This dramatically improves numerical
  stability for large or pathological matrices but is very resource-intensive
  for the compiler and will severely increase compile times.

Compiler flags:

* `-fconstexpr-steps` (clang) or `-fconstexpr-ops-limit` (gcc) - These flags
  increase the maximum number of steps the compiler will execute during
  constexpr evaluation. Computing eigenvalues requires substantial constexpr
  iterations, so these limits must typically be raised significantly to avoid
  compilation failure.

* `-fconstexpr-depth` - Increases the maximum depth of the constexpr call stack.
  Complex decompositions and iterations can lead to deep recursion or nested
  calls that exceed default limits (typically raised to `1024`).

* `-mfpmath=387` - (x86 specific) Directs the compiler to use the x87 FPU, which
  utilizes 80-bit internal precision. This can improve numeric stability and
  accuracy during compile-time evaluation and should be used in conjunction with
  `-mlong-double-80`.

* `-mlong-double-80` - (x86 specific) Ensures that `long double` is 80 bits.
  When combined with `CONSTEIG_USE_LONG_DOUBLE`, this provides extended
  precision to the constexpr algorithms, which is often crucial for the
  stability of operations like QR decomposition at compile time.

## Examples
### Population Flow

An example helps best. Let's say that we take the example from [Using
Eigenvectors to Find Steady State Population
Flows](https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764)
and apply it using `consteig`.

The `population.cpp` example demonstrates finding the eigenvalues of the population transition matrix:

```
Population Transition Matrix (A)
0.95 0.05
0.2 0.8

Eigenvalues (lambda):
1
0.75

To find the steady-state population distribution, we need to find the eigenvector 'v'
that corresponds to the eigenvalue of 1 by solving (A - lambda*I)v = 0.
```

As explained in the article, the eigenvalue of 1 is crucial for finding the
steady-state population. The next step, (which `consteig` does not attempt to
solve), would be the computation of the eigenvector corresponding to this
eigenvalue.

### Digital Filter Design

Another powerful application is Digital Filter Design. The `butterworth.cpp`
example demonstrates how to design a 2nd-order Butterworth digital filter using
the Zero-Order Hold (ZOH) method, but without algebraically performing a
Z-transform.

Instead of symbolically transforming the transfer function $H(s)$ to $H(z)$, we:
1.  Define the continuous-time state-space matrix $A_c$.
2.  Use `consteig` to find the continuous-time poles (eigenvalues of $A_c$).
3.  Map these poles directly to the Z-domain using $z = e^{sT}$ (Matched Z-Transform).
4.  Reconstruct the digital filter's characteristic polynomial from the mapped poles.

The `butterworth.cpp` example generates the following design for a 100Hz cutoff at 1kHz sampling:

```
Designing 2nd Order Butterworth Lowpass Filter
Cutoff: 100.00000000000000000000000000000000 Hz, Sampling Rate: 1000.00000000000000000000000000000000 Hz
Continuous System Matrix A:
[ 0.00000000000000000000000000000000, 1.00000000000000000000000000000000 ]
[ -394784.17604357434902340173721313476562, -888.57658763167319193598814308643341 ]

Continuous-time Poles (Eigenvalues of A):
p1 = -444.28829381583659596799407154321671 + j444.28829381583665281141293235123158
p2 = -444.28829381583659596799407154321671 + j-444.28829381583665281141293235123158

Discrete-time Poles (Mapped via z = exp(sT)):
z1 = 0.57902294991548219904586858319817 + j0.27563222764028760813914686877979
z2 = 0.57902294991548219904586858319817 + j-0.27563222764028760813914686877979

Filter Coefficients (Denominator):
a1 = -1.15804589983096439809173716639634
a2 = 0.41124070144277430349788460262062
Gain K = 0.25319480161180990540614743622427

Final Digital Filter Difference Equation:
y[n] = 0.25319480161180990540614743622427 * x[n] - (-1.15804589983096439809173716639634) * y[n-1] - (0.41124070144277430349788460262062) * y[n-2]
```

This approach numerically derives the filter coefficients by finding eigenvalues
directly, simplifying the design process for high-order filters where algebraic
transformation is tedious.

To demonstrate the compile time nature of this library, the filter data is
compiled into a special `.filter_data` section of the binary. You can compile
and extract the data as is shown below to demonstrate that the filter
coefficients are indeed calculated completly at compile time.

```
make butterworth.main
objcopy -O binary -j .filter_data build/examples/CMakeFiles/butterworth.main.dir/butterworth_values.cpp.o filter_data.bin
xxd -c 8 filter_data.bin
python3 examples/print_butterworth_binary.py
```

```
xxd -c 8 filter_data.bin

00000000: 0000 0000 0040 8f40  .....@.@
00000008: 0000 0000 0000 5940  ......Y@
00000010: 0000 0000 0000 0000  ........
00000018: 0000 0000 0000 f03f  .......?
00000020: 4ac4 44b4 8018 18c1  J.D.....
00000028: 84ea f9d9 9cc4 8bc0  ........
00000030: 84ea f9d9 9cc4 7bc0  ......{.
00000038: 85ea f9d9 9cc4 7b40  ......{@
00000040: 84ea f9d9 9cc4 7bc0  ......{.
00000048: 85ea f9d9 9cc4 7bc0  ......{.
00000050: 76a7 3023 5b87 e23f  v.0#[..?
00000058: 0c13 dc5a f5a3 d13f  ...Z...?
00000060: 76a7 3023 5b87 e23f  v.0#[..?
00000068: 0c13 dc5a f5a3 d1bf  ...Z....
00000070: 76a7 3023 5b87 f2bf  v.0#[...
00000078: afc5 de84 c451 da3f  .....Q.?
00000080: d727 1cf8 5734 d03f  .'..W4.?
```
```
python3 examples/print_butterworth_binary.py

Name         Double (64-bit)      Decimal
----------------------------------------------------------------------
fs           0000 0000 0040 8f40  1000.000000000000000
fc           0000 0000 0000 5940  100.000000000000000
A_c[0][0]    0000 0000 0000 0000  0.000000000000000
A_c[0][1]    0000 0000 0000 f03f  1.000000000000000
A_c[1][0]    4ac4 44b4 8018 18c1  -394784.176043574349023
A_c[1][1]    84ea f9d9 9cc4 8bc0  -888.576587631673192
p1_real      84ea f9d9 9cc4 7bc0  -444.288293815836596
p1_imag      85ea f9d9 9cc4 7b40  444.288293815836653
p2_real      84ea f9d9 9cc4 7bc0  -444.288293815836596
p2_imag      85ea f9d9 9cc4 7b40  444.288293815836653
z1_real      76a7 3023 5b87 e23f  0.579022949915482
z1_imag      0c13 dc5a f5a3 d13f  0.275632227640288
z2_real      76a7 3023 5b87 e23f  0.579022949915482
z2_imag      0c13 dc5a f5a3 d1bf  -0.275632227640288
a1           76a7 3023 5b87 f2bf  -1.158045899830964
a2           afc5 de84 c451 da3f  0.411240701442774
K            d727 1cf8 5734 d03f  0.253194801611810
```

## Why Does This Exist
Originally this library was developed to support a generic digital filter
library targeted at embedded systems in which the digital filter coefficients
could be calculated at compile time. In order to find the filter coefficients
it’s required to solve a multi order polynomial. Implementing a root finder
natively is tedious and error prone. Finding the roots of a polynomial can be
reframed as an eigenvalue problem which makes life easier. The matlab and octave
root finding functions actually operate in this way.

## How Is This Different
There are powerful open source C++ eigenvalues solvers already in existence
which are more robust, better optimized, and better tested. The caveat is that
they cannot currently calculate Eigenvalues at compile time.

Another key component to most eigenvalues solvers is the reliance on the
standard library. This is for good reason as the standard library, in particular
the STL containers, are powerful and solve many problems.  Unfortunately in some
systems, particularly for embedded systems the standard library isn’t available.

This provides a compile-time alternative.

## Algorithmic Approach and Optimizations [^1]

See [methods](docs/methods.md) for a discussion on the implementation specifics
for the numerical solvers implemented by `consteig`.

## Verification

See [verification](docs/verification.md) for a discussion on the verification
methods implemented to test this library.


## When To Use Consteig
* Eigenvalues (real or complex) need to be known at compile time.
* Eigenvalues need to be known and the standard library is unavailable.
* You need to manipulate static matrices at compile time.

## What Can Improve
* Declaring matrices can be initializer bracket hell. Refer to [this
  example](examples/matrix.cpp) for help.
* For large matrices compilation may be slow due to the inherent cost of
  `constexpr` evaluation.
* Currently the matrix [decompositions](examples/decomp.cpp) require square
  matrices.
* Support for non-square QR decomposition and optimized determinant/inverse
  calculations.

## Performance
* Unit testing _does_ leverage components of the standard library and Eigen, but
  the `consteig` library core does not.

## Development
Build dependencies rely on:
* gcc/g++ (C++17 support)
* cmake (>= 3.11)
* make
* Octave (optional, for test generation)

These are all packaged into a docker container. To build and test:

```
make container.pull
export MY_UID=$(id -u)
export MY_GID=$(id -g)
make container.make.build
make container.make.test
```

## References

[^1]: O'Hara, Keith. GCE-Math (Generalized Constant Expression Math) [GCEM](https://github.com/kthohr/gcem)
[^2]: Golub, G. H., & Van Loan, C. F. (2013). Matrix computations (4th ed.). Johns Hopkins University Press.
