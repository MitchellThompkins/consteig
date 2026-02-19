# Current State - 2026-02-17

## Progress
1.  **Matrix Operations**: Added `norm1` and `normInf` functions to `matrix/operations.hpp` for robust deflation testing.
2.  **QR Decomposition**: 
    *   Implemented `qr_hessenberg` in `matrix/decompositions/qr.hpp` using Givens rotations for O(N^2) performance on Hessenberg matrices.
    *   Restored Modified Gram-Schmidt `qr` for general matrices as it proved more stable for this specific codebase's constraints.
3.  **Eigensolver**:
    *   Implemented `balance` function in `eigen/eigen.hpp` to improve numerical stability.
    *   Implemented an iterative shifted QR algorithm with Wilkinson shifts.
    *   Successfully achieved `constexpr` compatibility for 5x5 non-symmetric matrices.
    *   Optimized `constexpr` operation count to stay within compiler limits (using `qr_hessenberg` and reduced but sufficient iteration counts).
4.  **Testing**:
    *   All tests passing (15/15), including `generated_data.test` with its 5x5 non-symmetric `static_assert` case.
    *   Updated QR tests to accommodate algorithm changes while maintaining property-based verification.

## Resolved Issues
*   `NaN` and division-by-zero in Householder QR (reverted to Gram-Schmidt for stability).
*   Out-of-bounds array access in QR loops.
*   `constexpr` operation limit exceeded (resolved via optimization).
*   Convergence failure in 5x5 non-symmetric case (resolved via balancing and robust shifts).

## Next Steps
1.  Consider re-implementing Francis Double-Shift QR for better efficiency on larger matrices (though O(N^3) is acceptable for small `constexpr` sizes).
2.  Clean up any remaining debug comments in the codebase.
