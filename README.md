![example workflow](https://github.com/mitchellthompkins/consteig/actions/workflows/main.yml/badge.svg)

## What Is This 
consteig is a constexpr template library which uses both constexpr functions and
template meta-programming to calculate the eigenvalues and corresponding eigen
vectors of a square matrix at compile-time. That is, the eigenvalues and
eigenvectors can be saved as `static constexpr` values and no processor
execution time is spent calculating them at run-time. consteig also allows for
compile time static matrix manipulation. To remove any external dependences
several constexpr math functions are implemented as well.

This is particularly powerful if there is information that would normally
require an offline tool (MATLAB, Python/scipy, etc...) to verify or compute, but
which could instead be expressed directly in source code and be verified or
computed automatically at every build.

All at compile time, consteig supports:

* Computation of real and complex eigenvalues and eigenvectors for symmetric and
  non-symmetric matrices.
* Matrix construction and manipulation, including common operations and decompositions.
* A selection of mathematical functions, including complex arithmetic. [^1]
* A strictly freestanding core with no dependence on the C++ standard library.

## How To Use consteig
consteig is a templated library and as such a user does not need to compile
anything separately. Simply `#include “consteig.hpp”` into the project. The
cmake files here are only to facilitate testing and development.

consteig also requires a C++ compiler which supports **C++17**.

Here are some examples to help get started:
* [Declaring a matrix](examples/matrix.cpp)
* [Matrix Arithmetic](examples/matrix.cpp)
* [Finding eigenvalues](examples/eigen.cpp)
* [Population flow](examples/population.cpp)
* [Butterworth Filter Design](examples/butterworth/butterworth_core.hpp)

### Build Options

consteig has a few options which can be modified. However, these defaults are
well tested and modifying them may have non-desirable results such as increased
compile times or numerical instability.

CMake Functions:

* `consteig_raise_compiler_limits(target1 target2 ...)` - A convenience
  CMake function (implemented with `function()`, not `macro()`, for proper
  variable scoping) that raises `-fconstexpr-ops-limit` (GCC),
  `-fconstexpr-steps` (Clang), and `-fconstexpr-depth` on one or more specific
  targets to accommodate heavy constexpr workloads. The library itself does not
  call this function — its deflation criterion keeps iteration counts within
  default compiler limits. However, users working with very large or pathological
  matrices may find it useful to call this on their own targets.

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

* `CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD` - Controls the stopping criterion
  for the matrix balancing step. A scaling is applied to a row/column only if
  it reduces the sum of the row and column norms by more than this factor.
  The default value of `0.95` is taken from Algorithm 2 of James, Langou &
  Lowery [^4]. Increasing it toward `1.0` runs more balancing iterations;
  decreasing it stops earlier.

* `CONSTEIG_USE_LONG_DOUBLE` - Forces all internal constexpr eigenvalue
  calculations to use `long double`. This dramatically improves numerical
  stability for large or pathological matrices but is very resource-intensive
  for the compiler and will severely increase compile times.

Compiler flags:

* `-fconstexpr-steps` (clang) or `-fconstexpr-ops-limit` (gcc) - These flags
  increase the maximum number of steps the compiler will execute during
  constexpr evaluation. The library's deflation criterion (which includes
  an absolute check against machine epsilon in addition to the standard relative
  check) keeps iteration counts low enough that default compiler limits are
  sufficient for the test suite. However, very large or pathological matrices
  may still benefit from raising these limits.

* `-fconstexpr-depth` - Increases the maximum depth of the constexpr call stack.
  Default limits are typically sufficient, but deeply nested computations on
  large matrices may require raising this (e.g., to `1024`).

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

Take the example from [Using Eigenvectors to Find Steady State Population
Flows](https://medium.com/@andrew.chamberlain/using-eigenvectors-to-find-steady-state-population-flows-cd938f124764)
and apply it using consteig. The transition matrix is fixed at compile time, so
the steady-state fractions can be stored as `static constexpr` values. At runtime,
computing the distribution for any total population is then a single multiply.
There is no need to iterate the transition matrix until it converges.

The [`population.cpp`](examples/population.cpp) example demonstrates this:

```
Population Transition Matrix (A)
0.95 0.2
0.05 0.8

Initial Population Vector (u0)
500000
500000

Eigenvalues (lambda):
1
0.75

To find the steady-state population distribution, we need to find the eigenvector 'v'
that corresponds to the eigenvalue of 1 by solving (A - lambda*I)v = 0.

Steady-State Population Flow (Total: 1000000):
Seattle:  800000 (80.00%)
Portland: 200000 (20.00%)

Steady-State Population Flow (Total: 2500000):
Seattle:  2000000 (80.00%)
Portland: 500000 (20.00%)
```

The same compile-time eigenvectors drive both outputs; no matrix iteration happens at runtime.

### Digital Filter Design

consteig can be used to automatically generate the IIR digital filter
coefficents from a time domain transfer function. They are derived at
compile-time and can be saved for use in the filter step when the actual
filtering takes place. Consider the 2nd-order Butterworth low-pass transfer
function in the continuous time domain:

$$H(s) = \frac{\omega_c^2}{s^2 + \sqrt{2}\,\omega_c\,s + \omega_c^2}$$

Instead of symbolically transforming $H(s)$ to $H(z)$ with the Z-transform, we
use the Zero-Order Hold (ZOH) method:
1.  Read the denominator coefficients of $H(s)$ directly into the companion-form state-space matrix $A_c$.
2.  Use consteig to find the continuous-time poles (eigenvalues of $A_c$).
3.  Map these poles directly to the Z-domain using $z = e^{sT}$ (Matched Z-Transform).
4.  Reconstruct the digital filter's characteristic polynomial from the mapped poles.

The [`butterworth_core.hpp`](examples/butterworth/butterworth_core.hpp) example generates
the following design for a 100Hz cutoff at 1kHz sampling:

```
> ./build/bin/butterworth.main
Designing 2nd Order Butterworth Lowpass Filter
Cutoff: 100.0000 Hz, Sampling Rate: 1000.0000 Hz
Continuous System Matrix A:
[ 0.0000, 1.0000 ]
[ -394784.1760, -888.5766 ]

Continuous-time Poles (Eigenvalues of A):
p1 = -444.2883 + j444.2883
p2 = -444.2883 + j-444.2883

Discrete-time Poles (Mapped via z = exp(sT)):
z1 = 0.5790 + j0.2756
z2 = 0.5790 + j-0.2756

Filter Coefficients (Denominator):
a1 = -1.1580
a2 = 0.4112
Gain K = 0.2532

Final Digital Filter Difference Equation:
y[n] = 0.2532 * x[n] - (-1.1580) * y[n-1] - (0.4112) * y[n-2]

```

To demonstrate the compile time nature of this library, the filter data is
compiled into a special `.filter_data` section of the binary. You can compile
and extract the data as is shown below to demonstrate that the filter
coefficients are indeed calculated completely at compile time.

```
> make butterworth.main
> objcopy -O binary -j .filter_data build/examples/CMakeFiles/butterworth.main.dir/butterworth/butterworth_values.cpp.o filter_data.bin

> xxd -c 8 filter_data.bin

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

> python3 examples/butterworth/print_butterworth_binary.py

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

### Control Theory

At compile time, consteig can validate that the chosen gains for a PID loop meet
the required performance requirements for a system. Consider the [DC Motor
Position: PID Controller
Design](https://ctms.engin.umich.edu/CTMS/index.php?example=MotorPosition&section=ControlPID)
, along with the [state space
model](https://ctms.engin.umich.edu/CTMS/index.php?example=MotorPosition&section=ControlStateSpace)
from the University of Michigan. The information known at compile time is:

* The motor parameters (and thus state space model)
* Chosen gains
* System performance requirements

When building the system with gains that do not meet the performance requirements,
`static_assert` can validate system gains by preventing compilation:

```
> make container.make.dc_motor_control.main
...
/Users/mitchellthompkins/workspace/consteig/examples/dc_motor_control.cpp:143:19: error: static assertion failed due to requirement 'check_settling_time(eigs_bad, POLE_LIMIT_FOR_SETTLING)': Scenario 2 REJECTED: Settling time less than 0.040 seconds [FAILED]
  143 |     static_assert(check_settling_time(eigs_bad, POLE_LIMIT_FOR_SETTLING),
      |                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/Users/mitchellthompkins/workspace/consteig/examples/dc_motor_control.cpp:145:19: error: static assertion failed due to requirement 'check_overshoot(eigs_bad, ZETA_LIMIT_FOR_OVERSHOOT)': Scenario 2 REJECTED: Overshoot less than 16% [FAILED]
  145 |     static_assert(check_overshoot(eigs_bad, ZETA_LIMIT_FOR_OVERSHOOT),
      |                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

```

This aligns with the output produced by octave:

```
> octave octave/dc_motor_control.m
...
State Feedback Gains: K = [21.0000, 0.0500, 0.0000, -200.0000]

Closed-Loop Poles:
  -1454381.2397 (real)
  -77.7191 +192.3253j  ->  zeta=0.3747
  -77.7191 -192.3253j  ->  zeta=0.3747
  -9.8632 (real)

Validation:
  [PASS] Stability check (All poles in LHP)
  [FAIL] Settling time less than 0.040 seconds (Actual: 0.0509 s)
  [FAIL] Overshoot less than 16% (Actual: 33.40 %)
  [PASS] No steady-state error (Enforced by integral action)

```

![Motor Control](docs/imgs/step_response.png "step_response")


## Why Does This Exist

This library was originally developed to support a generic digital filter
library for embedded systems, where filter coefficients are calculated at
compile time. Computing those coefficients requires solving a high-order
polynomial, which can be reframed as an eigenvalue problem, making it far more
manageable. This is the same approach used by
[LAPACK](https://netlib.org/lapack/), and by extension MATLAB and Octave's
root-finding functions.

## How Is This Different

Powerful open source C++ eigenvalue solvers already exist that are more robust,
optimized, and tested than anything here. However, they share two limitations:
they cannot compute eigenvalues at compile time, and they depend on the
standard library, which is unavailable on some embedded systems. This library
addresses both constraints.

## Algorithmic Approach and Optimizations [^3]

See [docs/methods.md](docs/methods.md) for a discussion on the implementation
specifics for the numerical solvers implemented by consteig.

## Verification, Accuracy and Performance

consteig uses `8x8` matrices as its test basis and leverages 2 tolerances for
verification. For all non-defective matrices it uses `1e-9` as an expectation
when comparing against a reference.  For highly defective matrices, as is the
case for Jordan blocks, it uses `0.03` [^4]. `0.03` approaches the numerical limit
of verification accuracy for 64-bit types.

Note that unit testing _does_ leverage components of the standard
library and Eigen, but the consteig library core does not.

See [docs/verification.md](docs/verification.md) for a detailed discussion on
the accuracy and verification methods implemented to test this library.


## When To Use consteig

* Eigenvalues (real or complex) need to be known at compile time.
* Eigenvalues need to be known and the standard library is unavailable.
* You need to manipulate static matrices at compile time.

## Development

### With Docker (recommended)

The only local dependency is Docker. All build tools are packaged in the dev container:

```
make container.pull
export MY_UID=$(id -u)
export MY_GID=$(id -g)
make container.make.build
make container.make.test
```

Any make target can be run inside the container via `container.make.<target>`.

### Without Docker (native)

Required:
* gcc and/or clang (C++17 support)
* cmake (>= 3.13)
* make
* clang-format (for `make format`)

Optional:
* Octave (for regenerating test cases via `make generate-test-cases`)

```
make build
make test
```

## References

[^1]: O'Hara, Keith. GCE-Math (Generalized Constant Expression Math) [GCEM](https://github.com/kthohr/gcem)
[^2]: James, R., Langou, J., & Lowery, B. R. (2014). [On matrix balancing and eigenvector computation](https://arxiv.org/pdf/1401.5766)
[^3]: Golub, G. H., & Van Loan, C. F. (2013). Matrix computations (4th ed.). Johns Hopkins University Press.
[^4]: Stewart, G. W., and J.-G. Sun. 1990. Matrix Perturbation Theory. Boston: Academic Press. §3.1.
