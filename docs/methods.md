---
title: Methods
---

# Algorithmic Approach and Optimizations [^2]

Consteig employs a hybrid approach to performance, balancing `constexpr`
compatibility with the use of robust and efficient numerical methods.

## Eigenvalue Calculation (Core Solver)
The core eigenvalue solver is based on the optimized **Francis QR
algorithm**, tailored for a `constexpr` context.

*   **Preprocessing Steps**:
    1.  **Balancing**: An initial balancing step permutes and scales the matrix
        to reduce the norm of its rows and columns. This is a crucial step for
        improving the accuracy and convergence rate of the subsequent QR
        iterations, especially for matrices with poorly scaled entries.
    2.  **Hessenberg Reduction**: The balanced matrix is then reduced to upper
        Hessenberg form using a series of Householder transformations. This is a
        critical optimization that reduces the computational cost of each QR
        iteration from O(n^4) to O(n^2), bringing the overall complexity of the
        eigenvalue problem down to a manageable O(n^3).

*   **QR Iteration Loop**:
    *   **Implicit Double-Shift QR**: The algorithm employs a true implicit
        double-shift strategy using Householder reflectors for bulge chasing.
        Rather than explicitly forming the shifted matrix product, the algorithm
        introduces a bulge in the Hessenberg form and chases it down the
        diagonal via a sequence of Householder reflections, preserving
        Hessenberg structure throughout. This allows the solver to find complex
        conjugate eigenvalues for real non-symmetric matrices without resorting
        to complex arithmetic during the iteration process.
    *   **Wilkinson Shifts**: To accelerate convergence, Wilkinson shifts are
        used as the default shifting strategy. This method provides a
        quadratically convergent rate in most cases.
    *   **Robust Deflation**: The algorithm checks for convergence by monitoring
        the sub-diagonal elements. Deflation occurs when an element becomes
        negligible relative to its neighboring diagonal elements, scaled by
        machine epsilon, effectively splitting the problem into smaller,
        independent sub-problems.
    *   **Exceptional Shifts**: To prevent stalling in cases where Wilkinson
        shifts fail to converge, the solver introduces LAPACK-style exceptional
        shifts every 10 iterations, alternating between top-based and
        bottom-based shift strategies to break out of convergence stalls.
    *   **Eigenvalue Verification**: After convergence, the solver verifies
        computed eigenvalues by checking both the trace (sum of eigenvalues
        equals the matrix trace) and determinant (product of eigenvalues equals
        the matrix determinant) invariants.

## Matrix Decompositions
*   **QR Decomposition**: The general-purpose `qr()` decomposition is
    implemented using a series of Givens rotations. This method was chosen
    for its numerical stability over alternatives like the Gram-Schmidt process,
    which can suffer from a loss of orthogonality in the Q matrix due to
    floating-point inaccuracies. This stability was essential for achieving
    high-precision results with tighter test tolerances. The specialized
    `qr_hessenberg()` also uses Givens rotations, optimized for the Hessenberg
    structure.

## Other Operations
*   **Determinant**: The `det()` function is currently implemented using Laplace
    expansion (cofactor expansion). While straightforward and
    `constexpr`-compatible, this algorithm has a factorial time complexity
    (O(n!)) and is only practical for very small matrices. This is a known
    trade-off for implementation simplicity.

## Comparison with LAPACK/Eigen
While `consteig` uses the same fundamental Francis QR algorithm as LAPACK
(`DLAHQR`) and Eigen, users may notice lower accuracy on highly defective
matrices (e.g., error $\approx 0.05$ vs $0.01$). This difference stems from
several factors:
1.  Balancing Strategy: `consteig` implements basic diagonal scaling balancing
    (`GEBAL` scaling only). Standard libraries also perform permutation to
    isolate eigenvalues, which significantly improves conditioning for reducible
    matrices.
2.  Arithmetic: This library uses real arithmetic with implicit double shifts to
    maintain `constexpr` compatibility and performance. Full complex arithmetic
    solvers can sometimes resolve clustered eigenvalues more cleanly.
3.  Blocking: Runtime libraries use blocked algorithms (Level 3 BLAS) that
    accumulate round-off errors more favorably than the unblocked Level 1/2
    algorithms required for straightforward `constexpr` implementation.
4.  Floating Point Environment: `constexpr` evaluation is performed by the
    compiler's abstract machine, which may not support the same denormal
    handling or extended precision intermediate registers (80-bit/128-bit) that
    a runtime hardware FPU might utilize to preserve precision in critical
    steps.

[^2]: Golub, G. H., & Van Loan, C. F. (2013). Matrix computations (4th ed.). Johns Hopkins University Press.
