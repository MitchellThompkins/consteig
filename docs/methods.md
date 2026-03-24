# Algorithmic Approach and Optimizations [^1]

consteig employs a hybrid approach to performance, balancing `constexpr`
compatibility with the use of robust and efficient numerical methods.

## Eigenvalue Calculation (Core Solver)

The core eigenvalue solver is based on the optimized Francis QR algorithm,
tailored for a `constexpr` context.

* Preprocessing Steps:
    1. Balancing: An initial balancing step applies diagonal scaling
       (Parlett & Reinsch 1969) to reduce the norm of rows and columns.
       This improves the accuracy and convergence rate of the subsequent QR
       iterations, especially for matrices with poorly scaled entries. No
       permutation-based eigenvalue isolation is performed.
    2. Hessenberg Reduction: The balanced matrix is then reduced to upper
       Hessenberg form using a series of Householder transformations. This is a
       critical optimization that reduces the computational cost of each QR
       iteration from O(n^3) to O(n^2), bringing the overall complexity of the
       eigenvalue problem down to a manageable O(n^3).

* QR Iteration Loop:
    * Implicit Double-Shift QR: The algorithm employs a true implicit
      double-shift strategy using Householder reflectors for bulge chasing.
      Rather than explicitly forming the shifted matrix product, the algorithm
      introduces a bulge in the Hessenberg form and chases it down the
      diagonal via a sequence of Householder reflections, preserving
      Hessenberg structure throughout. This allows the solver to find complex
      conjugate eigenvalues for real non-symmetric matrices without resorting
      to complex arithmetic during the iteration process.
    * Wilkinson Shifts: To accelerate convergence, Wilkinson shifts are
      used as the default shifting strategy. This method provides a
      quadratically convergent rate in most cases.
    * Robust Deflation: The algorithm checks for convergence by monitoring
      the sub-diagonal elements. Deflation occurs when an element becomes
      negligible relative to its neighboring diagonal elements, scaled by
      machine epsilon, effectively splitting the problem into smaller,
      independent sub-problems.
    * Exceptional Shifts: To prevent stalling in cases where Wilkinson
      shifts fail to converge, the solver introduces LAPACK-style exceptional
      shifts every 10 iterations, alternating between top-based and
      bottom-based shift strategies to break out of convergence stalls.
    * Eigenvalue Verification: After convergence, the solver verifies
      computed eigenvalues by checking both the trace (sum of eigenvalues
      equals the matrix trace) and determinant (product of eigenvalues equals
      the matrix determinant) invariants.

## Matrix Decompositions

* QR Decomposition: The general-purpose `qr()` decomposition is
  implemented using a series of Givens rotations. This method was chosen
  for its numerical stability over alternatives like the Gram-Schmidt process,
  which can suffer from a loss of orthogonality in the Q matrix due to
  floating-point inaccuracies. This stability was essential for achieving
  high-precision results with tighter test tolerances. The specialized
  `qr_hessenberg()` also uses Givens rotations, optimized for the Hessenberg
  structure.

## Other Operations

* Determinant: The `det()` function is currently implemented using Laplace
  expansion (cofactor expansion). While straightforward and
  `constexpr`-compatible, this algorithm has a factorial time complexity
  (O(n!)) and is only practical for very small matrices. This is a known
  trade-off for implementation simplicity.

## Comparison with LAPACK/Eigen

While consteig uses the same fundamental Francis QR algorithm as LAPACK
(`DLAHQR`) and Eigen, users may notice lower accuracy on highly defective
matrices. This difference stems from several factors:
1.  Balancing Strategy: consteig implements diagonal scaling only (based on
    Parlett & Reinsch 1969)[^2]. Standard libraries also perform permutation to
    isolate eigenvalues, which significantly improves conditioning for reducible
    matrices[^3].
2.  Arithmetic: To avoid complex arithmetic in the iteration loop, this library
    uses the implicit double-shift strategy, which tracks complex conjugate pairs
    together as a single 2x2 step. When conjugate pairs are clustered close to
    other eigenvalues, this coupling makes the solver more sensitive to
    perturbations. A complex arithmetic solver would track each eigenvalue
    independently, sidestepping this issue at the cost of roughly 4x more
    arithmetic per iteration.
3.  Floating Point Environment: `constexpr` evaluation is performed by the
    compiler's abstract machine, which does not use extended precision
    intermediate registers (80-bit/128-bit) that a runtime hardware FPU might
    utilize to preserve precision in critical steps. As far as I can tell,
    `constexpr` evaluation does not use extended floating-point registers even
    on hardware that supports them.

[^1]: Golub, G. H., & Van Loan, C. F. (2013). Matrix computations (4th ed.). Johns Hopkins University Press.
[^2]: Parlett, B. N., & Reinsch, C. (1969). Balancing a matrix for calculation of eigenvalues and eigenvectors. Numerische Mathematik, 13. Springer.
[^3]: LAPACK. [DGEBAL: Balance a general real matrix](https://netlib.org/lapack/explore-html/df/df3/group__gebal_ga999e2b7075625a2b5ce75e0241ecdb19.html)
