# Post-Mortem: Non-Symmetric Eigensolver Implementation

## Summary
The task of fixing convergence for 5x5 non-symmetric matrices took longer than expected due to a combination of algorithmic complexity, strict `constexpr` limitations, and an initial "over-engineering" of the solution.

## Core Issues

### 1. The "Francis QR" Rabbit Hole
My initial strategy was to implement the **Francis Implicit Double-Shift QR** algorithm. This is the industry standard (used in LAPACK/Eigen) because it's fast ($O(N^2)$ per iteration) and handles complex eigenvalue pairs entirely in real arithmetic.
*   **Complexity:** This algorithm requires precise "bulge chasing" logic involving 3x3 Householder reflections and careful index management.
*   **Implementation Errors:** I introduced subtle bugs in the bulge-chasing indices (out-of-bounds access on 2x2 blocks) and similarity transformations (updating only parts of rows/cols, breaking the similarity property $A_{new} = Q^T A Q$).
*   **Debugging Difficulty:** Since the code runs at compile-time (`constexpr`), I couldn't easily step through with a debugger or use print statements without converting tests to runtime, making the feedback loop very slow.

### 2. The Householder QR Detour
Suspecting the underlying QR decomposition was unstable, I replaced the existing **Modified Gram-Schmidt (MGS)** with **Householder QR**.
*   **Regression:** This broke existing tests because Householder QR produces a different $Q$ matrix (reflection vs. projection) than MGS. While valid, it required rewriting test expectations.
*   **Instability:** My initial Householder implementation had a division-by-zero edge case for identity-like columns, causing `NaN`s in the solver.

### 3. `constexpr` Constraints
C++17 `constexpr` is very strict.
*   **Initialization:** Every variable must be initialized.
*   **Control Flow:** Loops and branches must be deterministic constants.
*   **Operation Limits:** As I increased the iteration count to force convergence, I hit the compiler's maximum `constexpr` operation limit (33 million ops). This forced me to stop brute-forcing and optimize the algorithm.

## The Turning Point & Solution

I eventually pivoted to a simpler, more robust strategy:

1.  **Back to Basics:** I abandoned the complex Francis algorithm in favor of **Standard Iterative QR with Wilkinson Shifts**. While less efficient theoretically ($O(N^3)$ vs $O(N^2)$), it is far simpler to implement correctly and sufficient for small ($N \le 10$) matrices.
2.  **Matrix Balancing:** I implemented a `balance()` function. This was the "magic bullet" for convergence. It scales rows and columns to reduce the matrix norm, preventing numerical noise from drowning out small subdiagonal elements.
3.  **Optimization:** To satisfy the `constexpr` operation limit, I replaced the $O(N^3)$ full QR step with an $O(N^2)$ **Hessenberg QR** step using Givens rotations. This kept the iteration count high enough for convergence while keeping the compiler happy.

## Lessons Learned
*   **Algorithm Selection:** For small, fixed-size matrices, simpler algorithms (Standard QR) are often better than asymptotic optimal ones (Francis) due to lower implementation overhead and complexity.
*   **Pre-conditioning:** Matrix balancing is not optional for non-symmetric eigensolvers; it should have been the first step.
*   **Testing:** Property-based tests (checking $A = QR$ and $Q^T Q = I$) are more robust than value-based tests when algorithms might change.
