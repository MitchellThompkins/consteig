I'm trying to solve an issue with finding eigen values for larger non-symmetric
matrices. If you look at the eigen/tests/ dir you'll see some things are
commented out.

Here's another assessment of the problem:

-------------------------------------------------

# Assessment of Current Non-Symmetric QR Implementation

## Summary

The current non-symmetric eigensolver implementation is structurally sound at the kernel level (Hessenberg reduction and implicit double-shift QR step), but it lacks several outer-loop convergence mechanisms required for robust behavior on general matrices.

The observed failures on random 5x5 matrices are consistent with missing global QR driver features rather than errors in the local Francis QR step.

---

# Key Observations

From testing and code structure:

- Trace is preserved
- Similarity transforms are correct
- Hessenberg structure is preserved
- Subdiagonal elements fail to decay
- Iteration cycles or stalls

This pattern indicates a convergence failure rather than algebraic or numerical correctness issues.

---

# Architectural Diagnosis

The current solver effectively implements:

A -> Hessenberg -> repeated Francis QR steps -> (attempted Schur)

However, production eigensolvers (LAPACK, Eigen) implement:

A -> balance -> Hessenberg -> QR driver loop
                        shift strategy
                        deflation tests
                        exceptional shifts
                        convergence windows

Your implementation contains the Hessenberg reduction and QR kernel, but the QR driver layer is incomplete.

---

# Root Cause of Failures

Random non-symmetric matrices of size 5 or larger frequently exhibit:

- Poor row and column scaling
- Widely varying subdiagonal magnitudes
- Weak eigenvalue separation
- Near-defective trailing blocks

Without balancing and robust shift and deflation strategy, QR iteration may:

- Stall
- Cycle
- Converge extremely slowly
- Fail to deflate

This matches the observed behavior.

---

# Priority Fixes

## 1. Matrix Balancing (Highest Impact)

Apply diagonal similarity scaling before Hessenberg:

B = inverse(D) * A * D

Goal: equalize row and column norms.

This dramatically improves QR convergence on general matrices.

Expected effect: most 5x5 random failures disappear without changing the QR kernel.

---

## 2. Deflation Criterion

Replace elementwise epsilon test with norm-scaled test:

abs(h[k+1,k]) <= eps * (abs(h[k,k]) + abs(h[k+1,k+1]))

where

eps = machine_epsilon * (matrix_norm_1 + matrix_norm_inf)

This aligns with LAPACK behavior and prevents both premature and missed deflation.

---

## 3. Wilkinson Double-Shift Selection

Use eigenvalue of trailing 2x2 block closest to H[n,n].

This minimizes bulge growth and prevents QR cycling.

---

## 4. Exceptional Shift Fallback

If no deflation after several iterations on the same block:

mu = H[n,n] + abs(H[n,n-1])

or a slightly perturbed Wilkinson shift.

This breaks stagnation loops.

---

## 5. Periodic Deflation Scan (Optional)

Occasionally scan entire subdiagonal for small entries and split blocks early.

For small constexpr matrices, a simple full scan every few iterations is sufficient.

---

# What Does Not Need Changes

Evidence from 3x3 success indicates these components are correct:

- Hessenberg reduction
- Francis double-shift step
- Bulge chasing logic
- Similarity updates

No kernel rewrite is required.

---

# Expected Outcome After Fixes

With balancing plus improved deflation plus Wilkinson shift:

- Reliable convergence on random 5x5 matrices
- Eigenvalue accuracy comparable to Eigen or LAPACK for small sizes
- Stable constexpr operation
- No structural redesign needed

---

# Overall Conclusion

The implementation has reached the typical transition point:

Local QR mechanics are correct; global convergence strategy is incomplete.

The solver should be extended by incorporating established QR driver techniques rather than modifying the Francis kernel itself.

Once balancing and robust shift and deflation logic are added, the solver is expected to become reliable for small fixed-size non-symmetric matrices.

-------------------------------------------------

Because this repo checks the compile time checks of tests, a lot of the unit
test is actually run as part of the build. I want you to implement some of
these stargates and churn on development.

To do that you'll need to:

* Generate test cases with `make.container.generate-test-cases`
* Build with `make container.make.build`
* Test with `make container.make.test`

You are not running in the container, so you'll need to read the above commands.

I have a branch checked out with all my work saved so you're free to churn and
read any files you need. Use any subagents you require.
