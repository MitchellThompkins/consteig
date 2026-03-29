# Agent Guide for Consteig

> **Note:** `CLAUDE.md` and `GEMINI.md` are symlinks to this file (`AGENT.md`). Always edit `AGENT.md` directly — editing the symlinks will break them.

## Project Overview

Consteig is a **C++17 constexpr template library** for computing eigenvalues and eigenvectors of square matrices at compile time. The library is designed to be:
- **Freestanding**: No dependency on the C++ standard library in the core
- **Compile-time**: All computations can be done as `static constexpr`
- **Header-only**: Just `#include "consteig.hpp"`

## Directory Structure

```
consteig/
├── consteig.hpp              # Main include file
├── consteig_options.hpp      # User-configurable options
├── consteig_types.hpp        # Core type definitions (Size, etc.)
├── eigen/                    # Eigenvalue/eigenvector solvers
│   └── eigen.hpp            # Core eigenvalue algorithms
├── math/                     # Constexpr math functions
│   ├── complex.hpp          # Complex number support
│   ├── constmath.hpp        # Math function includes
│   └── functions/           # Individual math functions (sqrt, exp, etc.)
├── matrix/                   # Matrix operations
│   ├── matrix.hpp           # Matrix class definition
│   ├── operations.hpp       # Matrix arithmetic and operations
│   ├── decompositions.hpp   # Decomposition interface (includes subdir)
│   └── decompositions/      # Individual decomposition implementations
│       ├── householder.hpp  # Householder reflections
│       ├── hessenberg.hpp   # Hessenberg reduction
│       ├── qr.hpp           # QR decomposition
│       └── lu.hpp           # LU decomposition
├── tests/                    # All tests
│   ├── CMakeLists.txt       # Adds math, matrix, and eigen subdirs
│   ├── math/                # Math function tests
│   ├── matrix/              # Matrix operation tests
│   └── eigen/               # Eigenvalue/eigenvector tests
├── test_dependencies/        # Test utilities
│   ├── test_tools.hpp       # Test tolerance constants and helpers
│   ├── eigen_test_tools.hpp # Eigen library comparison utilities
│   ├── gtest_stubs/         # Test stub utilities
│   └── googletest/          # Google Test framework (submodule)
├── examples/                 # Usage examples
├── docs/                     # Documentation (MkDocs site)
│   ├── index.md             # Documentation home page
│   ├── mkdocs.yml           # MkDocs configuration
│   ├── methods.md           # Algorithm descriptions
│   ├── verification.md      # Testing methodology
│   ├── getting-started.md   # Installation and first steps
│   ├── guide/               # Detailed guides (matrix, eigensolvers, etc.)
│   └── examples/            # Example walkthroughs
├── profiling/                # Compile-time profiling
│   ├── run_profiling.sh     # Profiling script
│   ├── analyze_results.py   # Results analysis and plotting
│   └── compile_time/        # Generated profiling source files
├── cmake/                    # CMake support files
│   └── toolchains/          # Cross-compiler toolchain files (ARM)
└── octave/                   # Octave scripts for test generation
```

## Build System

### Build Commands

All builds run inside a Docker container for reproducibility:

```bash
# Pull the development container
make container.pull

# Build the project (includes static_assert compile-time tests)
make container.make.build

# Run runtime tests (parallel via ctest -j)
make container.make.test

# Build examples
make container.make.examples

# Format code
make format

# Regenerate Octave test cases (rarely needed — only when intentionally updating test data)
make generate-test-cases
```

### Key Build Notes

- **`static_assert` IS a test**: The library uses `static_assert` extensively. Build failures ARE test failures.
- Default compiler constexpr limits are sufficient thanks to the deflation criterion in the eigenvalue solver
- Tests are split into individual `.cpp` files to avoid exhausting compiler constexpr budgets

## Type System

### Core Types

- `consteig::Size` - Used for matrix dimensions (typically `std::size_t`)
- `consteig::Matrix<T, R, C>` - Fixed-size matrix template
- `consteig::Complex<T>` - Constexpr complex number type

### Type Consistency Rules

The `equalWithinMat` template function requires **all three parameters to have the same type T**:

```cpp
template <typename T, Size R, Size C>
constexpr bool equalWithinMat(Matrix<T, R, C> a, Matrix<T, R, C> b, const T thresh);
```

This means:
- `Matrix<double, ...>` must use a `double` tolerance (e.g., `CONSTEIG_TEST_TOLERANCE`)
- `Matrix<float, ...>` must use a `float` tolerance (e.g., `CONSTEIG_FLOAT_TEST_TOLERANCE`)

## Test Tolerance Constants

Defined in `test_dependencies/test_tools.hpp`:

See `test_dependencies/test_tools.hpp` for current values.

| Constant | Type | Use Case |
|----------|------|----------|
| `CONSTEIG_TEST_TOLERANCE` | double | Standard double-precision comparisons |
| `CONSTEIG_FLOAT_TEST_TOLERANCE` | float | Standard float-precision comparisons |
| `CONSTEIG_ITERATIVE_FLOAT_TOLERANCE` | float | Iterative methods with float matrices |
| `PATHOLOGICAL_TOL` | double | Defective/ill-conditioned matrices |
| `LARGE_VAL_TOL` | double | Large magnitude value comparisons |

### Choosing the Right Tolerance

1. **Direct computation tests** (non-iterative): Use `CONSTEIG_TEST_TOLERANCE` or `CONSTEIG_FLOAT_TEST_TOLERANCE`
2. **Iterative method tests** (Hessenberg, QR iteration): Use `CONSTEIG_ITERATIVE_FLOAT_TOLERANCE` for float matrices; use `CONSTEIG_TEST_TOLERANCE` for double matrices
3. **Defective matrices**: Use `PATHOLOGICAL_TOL`

## Strict Build Mode

The project enforces strict compiler warnings. When working with Eigen library comparisons:

- Use explicit `static_cast<Eigen::Index>(i)` when indexing Eigen matrices with `Size` variables
- This is required because `Size` (unsigned) to `Eigen::Index` (signed) is a narrowing conversion

Example:
```cpp
// Correct - explicit cast for strict builds
EXPECT_NEAR(sum(i, j),
            eigSum(static_cast<Eigen::Index>(i),
                   static_cast<Eigen::Index>(j)),
            CONSTEIG_TEST_TOLERANCE);
```

## Test Patterns

### Float vs Double Tests

Some tests intentionally test `float` precision, others test `double`. Preserve this distinction:

```cpp
// Float test - uses Matrix<float, ...> and float tolerance
TEST(householder, house_single)
{
    static constexpr Matrix<float, 2, 2> mat{...};
    static constexpr Matrix<float, 2, 2> answer{...};
    static_assert(equalWithinMat(test, answer, CONSTEIG_FLOAT_TEST_TOLERANCE), MSG);
}

// Double test - uses Matrix<double, ...> and double tolerance
TEST(householder, house)
{
    static constexpr Matrix<double, 10, 10> mat{...};
    static constexpr Matrix<double, 10, 10> answer{...};
    static_assert(equalWithinMat(test, answer, CONSTEIG_TEST_TOLERANCE), MSG);
}
```

### Compile-Time vs Runtime Tests

```cpp
// Compile-time verification (fails build if wrong)
static_assert(equalWithinMat(result, expected, TOLERANCE), MSG);

// Runtime verification (for Eigen comparisons)
ASSERT_TRUE(equalWithinMat(result, expected, TOLERANCE));
EXPECT_NEAR(computed, reference, TOLERANCE);
```

## Coding Style

- **Always use braces** for all control flow bodies (`if`, `else`, `for`, `while`), even single-line ones. No braceless statements.

## Common Pitfalls

1. **Type mismatch in tolerances**: Using `CONSTEIG_FLOAT_TEST_TOLERANCE` (float) with `Matrix<double, ...>` will fail to compile

2. **Missing explicit casts**: Forgetting `static_cast<Eigen::Index>()` when indexing Eigen matrices will fail strict builds

3. **Wrong tolerance for iterative methods**: Hessenberg and QR tests accumulate error; use `CONSTEIG_ITERATIVE_FLOAT_TOLERANCE` for float, `CONSTEIG_TEST_TOLERANCE` for double

4. **Modifying float/double test types**: Tests are intentionally typed; don't change `Matrix<float,...>` to `Matrix<double,...>`

## Algorithm Overview

The eigenvalue solver uses the **Francis QR algorithm**:

1. **Balancing**: Scale matrix to improve conditioning
2. **Hessenberg reduction**: Transform to upper Hessenberg form (O(n^3) speedup)
3. **QR iteration**: Implicit double-shift with Wilkinson shifts
4. **Deflation**: Split into smaller subproblems as eigenvalues converge

Matrix decompositions use **Givens rotations** for numerical stability.

## Accuracy Expectations

- **Well-conditioned matrices**: ~`1e-9` accuracy
- **Iterative methods**: ~`3e-4` accuracy (error accumulation)
- **Defective matrices**: ~`0.03` accuracy (theoretical limit for 8x8 Jordan blocks in double precision)

## Octave Scripts

The `octave/` directory contains GNU Octave/MATLAB scripts used for two purposes:

### Test Case Generation (`octave/generate_test_cases.m`)

This script generates C++ test data (matrices and reference eigenvalues/eigenvectors) that are verified at compile time via `static_assert`. It produces:
- `tests/eigen/generated_cases.hpp` — arrays of test matrices and expected results
- `tests/eigen/generated_*.test.cpp` — individual test files (one per case to stay within constexpr budgets)

Test categories generated include random symmetric/non-symmetric matrices plus robustness categories (defective, nearly defective, clustered eigenvalues, companion, Hamiltonian, etc.).

**This script rarely needs to be re-run.** The generated files are checked into the repository. Only regenerate when intentionally changing test parameters (matrix size, number of cases, or categories).

### Development Scripts (`octave/development_scripts/`)

Reference implementations of the algorithms used in the library (Householder, Hessenberg reduction, QR iteration, balancing, etc.). These are for prototyping and validating algorithm behavior, not for routine use.

### Example Script (`octave/dc_motor_control.m`)

A DC motor position control example that demonstrates a real-world use case for eigenvalue analysis — used alongside the C++ `examples/dc_motor_control.cpp` example.

## Merge Conflict Resolution Guidelines

When resolving conflicts between strict-build branches and main:

1. **Keep explicit casts** from strict-build for Eigen indexing
2. **Use tolerance constants** from main (not hardcoded values)
3. **Preserve float/double type distinctions** in tests
4. **Add new tolerance constants** to `test_tools.hpp` if needed for type consistency
