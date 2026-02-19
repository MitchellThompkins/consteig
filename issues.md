# Known Issues & Challenges

## Non-Symmetric Matrix Support

Implementing eigenvalue solvers for non-symmetric matrices is significantly more difficult than for symmetric ones. The current implementation relies on the **Single-Shift QR Algorithm**, which works well for symmetric matrices where eigenvalues are guaranteed to be real. However, non-symmetric matrices often have complex eigenvalues, requiring a different approach.

### 1. Complex Number Support (Prerequisite)
Non-symmetric real matrices can have **complex eigenvalues** (conjugate pairs). 
*   **Current State:** The library only supports real arithmetic (`float`, `double`, etc.).
*   **The Hurdle:** A full `constexpr` `Complex<T>` class is required. This involves implementing addition, multiplication, division, magnitude, and square root for complex numbers, all computable at compile time.

### 2. The Algorithm (Francis's Double-Shift)
To handle non-symmetric matrices, the **Double-Shift QR Algorithm** (Francis's Algorithm) is necessary.
*   **Real Arithmetic Logic:** To avoid expensive complex arithmetic, the algorithm performs two shifts simultaneously implicitly. This keeps operations in real numbers until the end.
*   **Bulge Chasing:** This involves "chasing" a $3 	imes 3$ bulge down the diagonal. The indexing and logic are notoriously tricky and error-prone compared to the symmetric case.

### 3. The Result (Real Schur Form)
The algorithm produces a **Real Schur Form** (Quasi-Upper Triangular matrix), not a diagonal one.
*   **Interpretation:** The resulting matrix contains:
    *   $1 	imes 1$ blocks on the diagonal (Real Eigenvalues).
    *   $2 	imes 2$ blocks on the diagonal (Complex Eigenvalue Pairs).
*   **Extraction:** Additional logic is needed to parse this matrix, solve the characteristic equation for each $2 	imes 2$ block, and return a vector capable of holding mixed real and complex results.

### 4. Compile-Time Constraints
*   **Recursion/Loops:** While C++14 supports loops, the double-shift algorithm is iterative and complex. Ensuring the compiler doesn't hit step limits or recursion depth limits during valid compile-time evaluation can be challenging.

### Summary
**Difficulty: High**

This requires implementing a completely different and mathematically complex solver, along with a foundational type system (Complex numbers) that doesn't currently exist in the library. It is widely considered one of the harder standard linear algebra algorithms to implement correctly from scratch.
