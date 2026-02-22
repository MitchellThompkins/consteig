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

It's important to note that `consteig` currently focuses exclusively on computing **eigenvalues**. For many applications, such as determining the stability of digital filters (as described in "Why Does This Exist"), the eigenvalues themselves are the desired result, and the eigenvectors are not required. Therefore, this library does not currently provide functionality for eigenvector computation.

### Example 1 - Population Flow

An example helps best. Let's say that we take the example from [Using Eigenvectors to Find Steady State Population Flows](https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764) and apply it using `consteig`.

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

### Example 2 - Digital Filter Design

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

To demonstrate the compile time nature of this library, the filter data
into a special `.filter_data` section of the binary. After building and
extracting said data with the below, it is demonstrated that all of these
calculations were performed by the compiler.

```sh
make butterworth.main
objcopy -O binary -j .filter_data build/examples/CMakeFiles/butterworth.main.dir/butterworth_values.cpp.o filter_data.bin
xxd -c 8 filter_data.bin
python3 examples/print_butterworth_binary.py
```

```sh
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
* [Butterworth Filter Design](examples/butterworth.cpp)

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

## Algorithmic Approach and Optimizations
Consteig employs a hybrid approach to performance, balancing `constexpr` compatibility with the use of robust and efficient numerical methods.

### Eigenvalue Calculation (Core Solver)
The core eigenvalue solver is based on the highly-optimized **Francis QR algorithm**, tailored for a `constexpr` context.

*   **Preprocessing Steps**:
    1.  **Balancing**: An initial balancing step permutes and scales the matrix to reduce the norm of its rows and columns. This is a crucial step for improving the accuracy and convergence rate of the subsequent QR iterations, especially for matrices with poorly scaled entries.
    2.  **Hessenberg Reduction**: The balanced matrix is then reduced to upper Hessenberg form using a series of Householder transformations. This is a critical optimization that reduces the computational cost of each QR iteration from O(n^4) to O(n^2), bringing the overall complexity of the eigenvalue problem down to a manageable O(n^3).

*   **QR Iteration Loop**:
    *   **Implicit Double-Shift QR**: The algorithm employs a true implicit double-shift strategy using Householder reflectors for bulge chasing. Rather than explicitly forming the shifted matrix product, the algorithm introduces a bulge in the Hessenberg form and chases it down the diagonal via a sequence of Householder reflections, preserving Hessenberg structure throughout. This allows the solver to find complex conjugate eigenvalues for real non-symmetric matrices without resorting to complex arithmetic during the iteration process.
    *   **Wilkinson Shifts**: To accelerate convergence, Wilkinson shifts are used as the default shifting strategy. This method provides a quadratically convergent rate in most cases, making the algorithm highly efficient.
    *   **Robust Deflation**: The algorithm checks for convergence by monitoring the sub-diagonal elements. Deflation occurs when an element becomes negligible relative to its neighboring diagonal elements, scaled by machine epsilon, effectively splitting the problem into smaller, independent sub-problems.
    *   **Exceptional Shifts**: To prevent stalling in cases where Wilkinson shifts fail to converge, the solver introduces LAPACK-style exceptional shifts every 10 iterations, alternating between top-based and bottom-based shift strategies to break out of convergence stalls.
    *   **Eigenvalue Verification**: After convergence, the solver verifies computed eigenvalues by checking both the trace (sum of eigenvalues equals the matrix trace) and determinant (product of eigenvalues equals the matrix determinant) invariants.

### Matrix Decompositions
*   **QR Decomposition**: The general-purpose `qr()` decomposition is implemented using a series of **Givens rotations**. This method was chosen for its excellent numerical stability over alternatives like the Gram-Schmidt process, which can suffer from a loss of orthogonality in the Q matrix due to floating-point inaccuracies. This stability was essential for achieving high-precision results with tighter test tolerances. The specialized `qr_hessenberg()` also uses Givens rotations, optimized for the Hessenberg structure.

### Other Operations
*   **Determinant**: The `det()` function is currently implemented using **Laplace expansion (cofactor expansion)**. While straightforward and `constexpr`-compatible, this algorithm has a factorial time complexity (O(n!)) and is only practical for very small matrices. This is a known trade-off for implementation simplicity.

### Comparison with LAPACK/Eigen
While `consteig` uses the same fundamental Francis QR algorithm as LAPACK (`DLAHQR`) and Eigen, users may notice lower accuracy on highly defective matrices (e.g., error $\approx 0.05$ vs $0.01$). This difference stems from several factors:
1.  **Balancing Strategy**: `consteig` implements basic diagonal scaling balancing (`GEBAL` scaling only). Standard libraries also perform **permutation** to isolate eigenvalues, which significantly improves conditioning for reducible matrices.
2.  **Arithmetic**: This library uses real arithmetic with implicit double shifts to maintain `constexpr` compatibility and performance. Full complex arithmetic solvers can sometimes resolve clustered eigenvalues more cleanly.
3.  **Blocking**: Runtime libraries use blocked algorithms (Level 3 BLAS) that accumulate round-off errors more favorably than the unblocked Level 1/2 algorithms required for straightforward `constexpr` implementation.
4.  **Floating Point Environment**: `constexpr` evaluation is performed by the compiler's abstract machine, which may not support the same denormal handling or extended precision intermediate registers (80-bit/128-bit) that a runtime hardware FPU might utilize to preserve precision in critical steps.

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
The library is verified through two primary methods:
1. **Eigen Library Comparison**: Unit tests link against the [Eigen](https://eigen.tuxfamily.org/) library to compare compile-time results against a high-performance reference implementation.
2. **Octave Test Generation**: An Octave script (`octave/generate_test_cases.m`) is provided to generate fresh matrix test data and expected results, which are automatically verified using `static_assert` at compile time.

### Compile-Time Verification Limits
Iterative algorithms like the QR iteration used here are computationally expensive for a compiler's `constexpr` evaluator. To reliably verify 100+ test cases without crashing the compiler or hitting operation limits, the test suite is divided into **Fast** and **Slow** variants:

*   **Fast Tests (4x4 Matrices)**: These are the default tests. They are small enough to converge quickly within standard compiler limits and provide a baseline for algorithm correctness.
*   **Slow Tests (8x8 Matrices)**: These are exhaustive tests for larger matrices. They require significantly higher `constexpr` step limits (1 billion) and are only enabled on request.

**Running Slow Tests**:
To build the slow variant of the tests, use the following command:
```bash
BUILD_SLOW_TESTS=1 make container.make.build
```

### Robustness Test Suite
In addition to random matrix tests, a dedicated robustness test suite exercises the solver against 13 categories of numerically challenging matrices:

*   Defective, nearly defective, non-normal, clustered eigenvalues, repeated eigenvalues, companion, graded, large Jordan blocks, Toeplitz, nearly reducible, random non-normal, Hamiltonian, and sparse interior matrices.

Each category includes 10 test cases (each covering both a fast 4x4 and slow 8x8 variant), totaling **130 test files**.

### Robustness & Accuracy Limitations

The library performs rigorous eigenvalue verification using both trace/determinant checks and direct comparison against reference values (generated via Octave/LAPACK).

#### Defective Matrices
For **defective matrices** (those with non-trivial Jordan blocks), the eigenvalue problem is inherently ill-conditioned. A perturbation of size $\epsilon$ in the matrix entries can result in a perturbation of size $\epsilon^{1/k}$ in the eigenvalues, where $k$ is the size of the Jordan block.

For an $8 \times 8$ defective matrix (Jordan block of size 8) in double precision ($\epsilon \approx 10^{-16}$):
$$ \text{Error} \approx (10^{-16})^{1/8} = 10^{-2} = 0.01 $$

Consequently, tests for **defective**, **nearly defective**, and **large Jordan block** matrices use a relaxed tolerance (`0.05`) to account for this theoretical limit. This does not indicate a bug in the algorithm, but rather the fundamental limit of computing eigenvalues for such matrices using standard double-precision arithmetic.

#### Standard Matrices
For normal, symmetric, and well-conditioned non-symmetric matrices, the library maintains high precision, typically matching reference values within `1e-9` or better.

**Running Robustness Tests**:
```bash
CONSTEIG_ENABLE_ROBUSTNESS=1 make container.make.build
```

**CI/CD Integration**:
*   **Fast Tests** run on every commit and pull request.
*   **Slow Tests** are automatically executed in CI only on the `main` and
    `develop` branches to ensure rigorous verification before release while
    maintaining fast feedback for active development.
*   **Granular Binaries**: Non-symmetric test cases are split into individual
    `.cpp` files. This ensures each `static_assert` gets a fresh "budget" of
    compiler operations and limits the memory overhead to a single matrix solve
    at a time.
*   **Resource Management**: Compiling these tests can be RAM-intensive.
    The build system automatically uses `make -j 1` when `BUILD_SLOW_TESTS=1` is set to prevent multiple compiler
    instances from exhausting system memory.
*   **Compiler Flags**: Unit test targets automatically raise limits like `-fconstexpr-ops-limit` locally to accommodate the depth of these calculations. For other targets, these limits are not modified by default to avoid unexpected side effects on user compiler configurations. Users can explicitly enable these raised limits globally by setting the `CONSTEIG_RAISE_COMPILER_LIMITS` CMake option to `ON`.

*   **Spectral Limits & Matrix Size Constraints**: While the algorithm is algebraically sound for higher orders, random 10x10 matrices (and larger) frequently encounter particular arrangement, spacing, or clustering of eigenvalues in the matrix configurations. They thusly fail to converge within even an expanded `constexpr` operation budget (1BM+ steps). These limits are applied automatically to library tests, but users may need to explicitly enable them for their own targets using `CONSTEIG_RAISE_COMPILER_LIMITS=ON`.
*   From a numerical analysis perspective, the 8x8 limit is necessitated by the following factors:
    *   **Spectral Separation and Convergence Rate**: The convergence rate of
        the QR algorithm is intrinsically governed by the ratios of adjacent
        eigenvalues $|\lambda_{i+1}/\lambda_i|$ in the ordered spectrum. As the
        matrix dimension $n$ increases, the probability of encountering
        "clustered" eigenvalues (where $\lambda_i \approx \lambda_{i+1}$) rises
        sharply. In a random real 10x10 matrix, eigenvalues often distribute
        near the unit circle with several close pairs or nearly equal
        magnitudes.
    *   **The Unit Convergence Factor**: When eigenvalues are poorly separated,
        the convergence factor $\rho = |\lambda_{i+1}/\lambda_i|$ approaches
        unity. This results in slow linear convergence, requiring an explosive
        number of iterations to satisfy the deflation criterion ($|h_{k+1,k}|
        \le \epsilon(|h_{kk}| + |h_{k+1,k+1}|)$).
    *   **Non-Normal Structure**: Randomly generated matrices are typically
        highly non-normal ($AA^* \neq A^*A$). Non-normality can lead to
        transient growth in the QR residual and further stall the convergence of
        sub-diagonal elements toward the Real Schur Form.
    *   **Constexpr Step Budget**: While 500M operations is significantly higher
        than the compiler default (33,554,432 in GCC), it remains a finite
        constraint. A stalled QR iteration on a 10x10 non-normal matrix can
        easily exhaust this budget, causing a compilation failure.

## What Can Improve
* Declaring matrices can be initializer bracket hell. Refer to [this example](examples/matrix.cpp)
  for help.
* For large matrices compilation may be slow due to the inherent cost of `constexpr` evaluation.
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

To run the robustness tests:
```
CONSTEIG_ENABLE_ROBUSTNESS=1 make container.make.build
```

## References

## Thanks
Development began by leveraging [gcem](https://github.com/kthohr/gcem) for all
of the constexpr math. However, as it depends on the standard library, it's
usage was temporary. That constexpr math implementations are inspired from the
gcem implementations.
