title: Configuration

# Configuration

## User Macros

All macros are guarded by `#ifndef` and can be overridden before including `consteig.hpp` or via compiler flags (`-DMACRO=value`).

### CONSTEIG_MAX_ITER

Maximum QR iterations per eigenvalue block. The solver runs at most `CONSTEIG_MAX_ITER * S` total iterations for an `S×S` matrix.

```cpp
#define CONSTEIG_MAX_ITER 1000  // increase for difficult matrices
#include "consteig.hpp"
```

**Default**: `500`
**Trade-off**: More iterations may help difficult matrices converge, but increases compile time and the risk of hitting compiler constexpr step limits.

### CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE

Routing threshold that determines whether a matrix is treated as symmetric. If `isSymmetric(threshold)` is `true`, the faster single-shift QR algorithm is used.

```cpp
#define CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE 1e-9  // tighter symmetry check
#include "consteig.hpp"
```

**Default**: `1e-6`

### CONSTEIG_BALANCE_CONVERGENCE_THRESHOLD

Controls the stopping criterion for the matrix balancing step. A scaling is applied only if it reduces the sum of row and column norms by more than this factor.

**Default**: `0.95` (from Algorithm 2 of James, Langou & Lowery 2014)
**Range**: Increase toward `1.0` for more balancing; decrease to stop earlier.

### CONSTEIG_TRIG_MAX_ITER

Maximum Taylor series iterations for `sin`, `cos`, `tan`, etc.

**Default**: `20` (14 suffice for `double` precision; 20 gives a comfortable margin)

### CONSTEIG_USE_LONG_DOUBLE

When defined, all internal eigenvalue calculations use `long double` instead of `double`, improving numerical stability for large or pathological matrices at the cost of significantly increased compile times.

```cpp
#define CONSTEIG_USE_LONG_DOUBLE
#include "consteig.hpp"
```

**Default**: Not defined.

## Compiler Flags

### Raising Constexpr Limits

For very large or pathological matrices that exceed default compiler limits:

```bash
# GCC
-fconstexpr-ops-limit=1000000000

# Clang
-fconstexpr-steps=1000000000

# Both
-fconstexpr-depth=1024
```

A convenience CMake function is provided for per-target limit increases:

```cmake
consteig_raise_compiler_limits(my_target)
```

### Extended Precision (x86)

On x86 platforms, these flags enable 80-bit extended precision during constexpr evaluation, which can significantly improve accuracy for difficult matrices:

```bash
-mfpmath=387       # Use x87 FPU (80-bit internal precision)
-mlong-double-80   # Ensure long double is 80 bits
```

Combine with `CONSTEIG_USE_LONG_DOUBLE` for maximum precision.

## When to Change Defaults

The defaults work for the full test suite (8×8 matrices, including defective Jordan blocks) without modification. Consider changing them only if you encounter:

- **Compile errors about constexpr step limits** → Raise `-fconstexpr-ops-limit` or `-fconstexpr-steps`
- **Convergence failures on large matrices** → Increase `CONSTEIG_MAX_ITER`
- **Poor accuracy on near-defective matrices** → Enable `CONSTEIG_USE_LONG_DOUBLE`
- **Very tight symmetry requirements** → Tighten `CONSTEIG_DEFAULT_SYMMETRIC_TOLERANCE`
