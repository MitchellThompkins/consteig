# Pull Request: Build System Simplification and Enhanced Test Coverage

## Overview
This PR simplifies the build system structure by flattening the build directory and significantly expands the test suite for core mathematical and matrix functions to cover edge cases, limits, and mathematical properties.

## Key Changes

### 1. Build System Simplification
-   **Modified File:** `Makefile`
-   **Action:** 
    -   Removed the mandatory `Debug/Release` build type subdirectories. The project now defaults to building directly in `build/`.
    -   Simplified `BUILD_PREFIX` and `INSTALL_PREFIX` logic.
    -   Updated CMake generation to reflect the flattened directory structure (referencing `..` instead of `../..`).
    -   Cleaned up the help menu to remove obsolete `BUILD_TYPE` options.

### 2. Expanded Math Function Testing
-   **Files:** `math/tests/abs.test.cpp`, `math/tests/pow.test.cpp`, `math/tests/sqrt.test.cpp`
-   **Action:** 
    -   **`abs()`**: Added verification for floating-point limits (`numeric_limits::max`, `min`, `lowest`), integer overflow scenarios, and `long double` precision.
    -   **`pow()`**: Added test cases for zero base, zero exponent, negative bases with integer exponents, and large integer powers.
    -   **`sqrt()`**: Added tests for basic types (`float`, `double`, `long double`), integer square roots (including floor behavior for non-perfect squares), negative inputs, and extremely large values.

### 3. Enhanced Matrix Decomposition Verification
-   **Files:** `matrix/tests/qr.test.cpp`, `matrix/tests/householder.test.cpp`
-   **Action:**
    -   **QR Decomposition**: Added tests for specific matrix types (Identity, Zero, Diagonal, Singular). Implemented explicit verification for the orthogonality of $Q$ ($Q^T Q = I$) and the reconstruction of the original matrix ($Q \times R = A$).
    -   **Householder Reflection**: Added a properties test to verify that the generated reflection matrix $P$ is both symmetric ($P = P^T$) and orthogonal ($P^T P = I$).
    -   **Standardized Tolerance**: Introduced a unified `kTol` (1e-9) for floating-point comparisons across matrix tests.

## Verification Results
-   **Build Status:** Successful using the simplified `make build` command.
-   **Test Status:** All unit tests (including newly added cases) passed successfully.
