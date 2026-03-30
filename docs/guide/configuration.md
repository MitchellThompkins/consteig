---
title: Configuration
---

# Configuration

consteig has a few options which can be modified. However, these defaults are
well tested and modifying them may have non-desirable results such as increased
compile times or numerical instability.

## User Macros

All macros are guarded by `#ifndef` and can be overridden before including
`consteig.hpp` or via compiler flags (`-DMACRO=value`).

### CONSTEIG_MAX_ITER

Controls the maximum number of iterations allowed for eigenvalue solvers.
Increasing this may help difficult matrices converge, but significantly increases
compile times and the likelihood of hitting compiler step limits.

```cpp
#define CONSTEIG_MAX_ITER 1000  // increase for difficult matrices (see consteig_options.hpp for default)
#include <consteig/consteig.hpp>
```

### CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE

A routing threshold used to determine if a matrix is "symmetric enough" to use
the optimized symmetric eigenvalue solver (`eig_shifted_qr`). If the matrix
symmetry exceeds this tolerance, the library falls back to the more robust but
heavier non-symmetric solver (`eig_double_shifted_qr`).

```cpp
#define CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE 1e-9  // tighter than default (see consteig_options.hpp for default)
#include <consteig/consteig.hpp>
```

### CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD

Controls the stopping criterion for the matrix balancing step. A scaling is
applied to a row/column only if it reduces the sum of the row and column norms
by more than this factor. The default value of `0.95` is taken from Algorithm 2
of James, Langou & Lowery [^1]. Increasing it toward `1.0` runs more balancing
iterations; decreasing it stops earlier.

### CONSTEIG_TRIG_MAX_ITER

Maximum Taylor series iterations for `sin`, `cos`, `tan`, etc. (default: see
`consteig_options.hpp`). 14 should suffice for `double` precision but the
library uses a slightly higher default for good margin.

### CONSTEIG_USE_LONG_DOUBLE

Forces all internal constexpr eigenvalue calculations to use `long double`. This
dramatically improves numerical stability for large or pathological matrices but
is very resource-intensive for the compiler and will severely increase compile
times. It is _NOT_ defined by default.

```cpp
#define CONSTEIG_USE_LONG_DOUBLE
#include <consteig/consteig.hpp>
```

## CMake Functions

### consteig_raise_compiler_limits

A convenience CMake function (implemented with `function()`, not `macro()`, for
proper variable scoping) that raises `-fconstexpr-ops-limit` (GCC),
`-fconstexpr-steps` (Clang), and `-fconstexpr-depth` on one or more specific
targets to accommodate heavy constexpr workloads. The library itself does not
call this function. Its deflation criterion keeps iteration counts within
default compiler limits. However, users working with very large or pathological
matrices may find it useful to call this on their own targets.

```cmake
consteig_raise_compiler_limits(my_target)
```

## Compiler Flags

### Raising Constexpr Limits

`-fconstexpr-steps` (Clang) or `-fconstexpr-ops-limit` (GCC) increase the
maximum number of steps the compiler will execute during constexpr evaluation.
The library's deflation criterion (which includes an absolute check against
machine epsilon in addition to the standard relative check) keeps iteration
counts low enough that default compiler limits are sufficient for the test
suite. However, very large or pathological matrices may still benefit from
raising these limits.

`-fconstexpr-depth` increases the maximum depth of the constexpr call stack.
Default limits are typically sufficient, but deeply nested computations on
large matrices may require raising this (e.g., to `1024`).

```bash
# GCC
-fconstexpr-ops-limit=1000000000

# Clang
-fconstexpr-steps=1000000000

# Both
-fconstexpr-depth=1024
```

### Extended Precision (x86)

`-mfpmath=387` (x86 specific) directs the compiler to use the x87 FPU, which
utilizes 80-bit internal precision. This can improve numeric stability and
accuracy during compile-time evaluation and should be used in conjunction with
`-mlong-double-80`.

`-mlong-double-80` (x86 specific) ensures that `long double` is 80 bits. When
combined with `CONSTEIG_USE_LONG_DOUBLE`, this provides extended precision to
the constexpr algorithms, which is often crucial for the stability of operations
like QR decomposition at compile time.

```bash
-mfpmath=387       # Use x87 FPU (80-bit internal precision)
-mlong-double-80   # Ensure long double is 80 bits
```

Combine with `CONSTEIG_USE_LONG_DOUBLE` for maximum precision.

## When to Change Defaults

The defaults work for the full test suite (8x8 matrices, including defective
Jordan blocks) without modification. Consider changing them only if you
encounter:

- Compile errors about constexpr step limits: Raise `-fconstexpr-ops-limit` or `-fconstexpr-steps`
- Convergence failures on large matrices:  Increase `CONSTEIG_MAX_ITER`
- Poor accuracy on near-defective matrices:  Enable `CONSTEIG_USE_LONG_DOUBLE`
- Very tight symmetry requirements: Tighten `CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE`

[^1]: James, R., Langou, J., & Lowery, B. R. (2014). [On matrix balancing and eigenvector computation](https://arxiv.org/pdf/1401.5766)
