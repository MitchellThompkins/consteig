# Non-Symmetric Eigenvalue Solver Issues

## Overview
The current implementation of the non-symmetric eigenvalue solver (`eig_double_shifted_qr`) in `consteig` successfully handles small (3x3) matrices but exhibits instability and convergence failures for larger (5x5) random matrices. This document details the observed behavior, potential causes, and attempted fixes.

## Observed Behavior

### 1. 3x3 Matrices
- **Status**: **Passing**.
- **Behavior**: The solver consistently converges to the correct eigenvalues. Runtime verification against the Eigen library (`eigen_comparison.test.cpp`) passes for random 3x3 non-symmetric matrices.

### 2. 5x5 Matrices
- **Status**: **Failing**.
- **Behavior**:
    - **Convergence Failure**: The algorithm often reaches the maximum iteration count (`100 * S`) without fully reducing the matrix to Real Schur Form.
    - **Incorrect Eigenvalues**: The computed eigenvalues (diagonal blocks of the returned matrix) differ significantly from reference values (e.g., Octave/LAPACK).
        - Example mismatch: Computed `1.88`, Expected `2.12`.
    - **Correct Trace**: Interestingly, the sum of the computed eigenvalues matches the trace of the original matrix (e.g., Computed Sum `2.64`, Expected Sum `2.64`).
        - **Implication**: The similarity transformations $A_{k+1} = Q^T A_k Q$ are being applied correctly (preserving the trace), but the algorithm is stalling or cycling before eliminating the sub-diagonal elements.

## Hypothesized Causes

### 1. Lack of Matrix Balancing
Standard eigenvalue solvers (like LAPACK's `DGEEV`) apply **balancing** before the QR algorithm. Balancing scales the rows and columns to make their norms comparable, which significantly improves the numerical stability and accuracy of the eigenvalue computation.
- **Current State**: `consteig` does **not** implement balancing.
- **Impact**: Without balancing, rounding errors can accumulate, and the sub-diagonal elements may not become small enough to satisfy the deflation criteria (`epsilon * (|a_ii| + |a_jj|)`), causing the loop to run indefinitely or return an unfinished matrix.

### 2. Shift Strategy Limitations
The implementation uses the standard **Francis Double Shift**. While generally robust, it can enter cycles for certain matrix structures.
- **Attempted Fix**: An "Exceptional Shift" strategy was implemented (forcing a random/different shift every 10 iterations) to break cycles.
- **Result**: This caused regressions in the previously working 3x3 cases and did not fully resolve the 5x5 convergence issues, leading to its reversion.

### 3. Deflation Logic and Bulge Chasing
There was a suspected off-by-one error in the bulge-chasing loop (`k < n - 2` vs `k < n - 1`).
- **Attempted Fix**: Updating the loop to `k < n - 1` and adding bounds checks.
- **Result**: This change introduced instability and regressions in 3x3 cases, suggesting the original loop structure might have been correct for the specific 3x3 bulge implementation, or the fix interacted poorly with the existing logic.

## Current State
- The `eig_double_shifted_qr` function is in its original state (working for 3x3).
- The test case `generated_tests.nonsymmetric_constexpr` (checking a 5x5 matrix) is **disabled** (commented out) in `eigen/tests/generated.test.cpp` with a TODO note.
- The runtime comparison test `eigen_comparison.random_nonsymmetric_5x5` is also **disabled**.

## Recommendations for Future Work
1.  **Implement Matrix Balancing**: This is the most likely missing piece for robust convergence on random matrices.
2.  **Robust Shift Strategy**: Port a more sophisticated shift strategy (e.g., from `Eigen` or `LAPACK`) that handles exceptional cases more gracefully.
3.  **Conservative Convergence**: Consider loosening the convergence tolerance or using a relative tolerance based on the matrix norm rather than individual element magnitudes.
