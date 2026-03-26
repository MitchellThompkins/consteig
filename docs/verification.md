---
title: Verification & Accuracy
---

# Verification

The library is verified through two primary methods:
1. Eigen Library Comparison: Unit tests link against the
   [Eigen](https://eigen.tuxfamily.org/) library to compare compile-time results
   against a high-performance reference implementation.
2. Octave Test Generation: An Octave script (`octave/generate_test_cases.m`)
   is provided to generate fresh matrix test data and expected results, which
   are automatically verified using `static_assert` at compile time.

Running Robustness Tests:

```bash
make container.make.build
make container.make.test
```

CI/CD Integration:
* Granular Binaries: Test cases are split into individual `.cpp` files. This
  ensures each `static_assert` gets a fresh "budget" of compiler operations and
  limits the memory overhead to a single matrix solve at a time.

## Accuracy [^1]

An eigenvalue solver is limited not only by the numerical algorithm it employs
but also by the conditioning of the eigenvalue problem and the finite precision
of the machine representation. For defective matrices, eigenvalues are
intrinsically ill-conditioned. That is, small perturbations to the matrix can
produce much larger perturbations in the eigenvalues.

For a defective eigenvalue associated with a single $N\times N$ Jordan block,
classical perturbation theory shows that eigenvalue perturbations scale as

$$|\delta \lambda| \sim \|E\|^{1/N}$$

Modern dense eigenvalue algorithms (e.g., QR) are backward stable, meaning
the perturbation they introduce is on the order of machine precision:

$$\|E\|\lesssim \epsilon_{\text{mach}}\|A\|$$

which says

> the numerical error in the matrix is no larger than machine epsilon times the
size of the matrix.

If the matrix is scaled so that its norm is about 1:

$$\|A\| \approx 1$$

then the backward error simplifies to:

$$\|E\| \approx \epsilon_{\text{mach}}$$

So the solver is effectively perturbing the matrix at the level of machine
precision.

For IEEE-754 double precision $\epsilon_{\text{mach}}=2^{-53}$ and a single
$8\times8$ Jordan block this gives:

$$(2^{-53})^{1/8} \approx 0.010$$

Accordingly, for defective matrices of this type, eigenvalues cannot in general
be expected to be accurate beyond the percent level in double precision. The
observed accuracy of ~0.03 for consteig on such matrices is therefore
consistent with the theoretical limit.

## Compile-Time Verification Limits

Iterative algorithms like the QR iteration used here are computationally
expensive for a compiler's `constexpr` evaluator. The library's deflation
criterion adds an absolute check against machine epsilon alongside the
standard relative check, which allows near-zero sub-diagonal elements to deflate
early. This dramatically reduces iteration counts, keeping the constexpr
operation budget within default compiler limits for the test suite.

## Robustness Test Suite

In addition to random matrix tests, a dedicated robustness test suite exercises
the solver against the following categories of numerically challenging matrices:

* Defective
* Nearly defective
* Non-normal
* Clustered eigenvalues
* Repeated eigenvalues
* companion
* graded
* Large Jordan blocks
* Toeplitz
* Nearly reducible
* Random non-normal
* Hamiltonian
* Sparse interior matrices.

## Robustness & Accuracy Limitations

The library performs eigenvalue verification using both trace/determinant checks
and direct comparison against reference values (generated via Octave/LAPACK).

### Defective Matrices

As discussed above, for defective matrices, the eigenvalue problem is
inherently ill-conditioned. A perturbation of size $\epsilon$ in the matrix
entries can result in a large perturbation of size in the eigenvalues.

Consequently, tests for "pathological" matrices use a relaxed tolerance to
account for this theoretical limit. This describes a fundamental limit of
computing eigenvalues for such matrices using standard double-precision
arithmetic.

### Standard Matrices

For normal, symmetric, and well-conditioned non-symmetric matrices, the library
maintains high-precision, typically matching reference values within `1e-9` or
better.

### Convergence Limits

Thanks to the deflation criterion, default compiler constexpr limits are
sufficient for the test suite. However, random matrices beyond 8x8 frequently
encounter clustering or poor separation of eigenvalues, causing QR iteration to
fail to converge within even an expanded `constexpr` operation budget (1B+
steps). Users working with larger matrices may need to raise compiler constexpr
limits on their own targets (see the [Build Options](../README.md#build-options)).

From a numerical analysis perspective, the following factors have a significant
impact:

* Spectral Separation and Convergence Rate: The convergence rate of the QR
  algorithm is intrinsically governed by the ratios of adjacent eigenvalues
  $|\lambda_{i+1}/\lambda_i|$ in the ordered spectrum. As the matrix
  dimension $n$ increases, the probability of encountering "clustered"
  eigenvalues (where $\lambda_i \approx \lambda_{i+1}$) rises sharply. For
  example, in a random real 10x10 matrix, eigenvalues often distribute near
  the unit circle with several close pairs or nearly equal magnitudes.
* The Unit Convergence Factor: When eigenvalues are poorly separated,
  the convergence factor $\rho = |\lambda_{i+1}/\lambda_i|$ approaches
  unity. This results in slow linear convergence, requiring an explosive
  number of iterations to satisfy the deflation criterion
  $|{h_{k+1,k}}| \le \epsilon (|h_{kk}| + |h_{k+1,k+1}|)$
* Non-Normal Structure: Randomly generated matrices are typically
  highly non-normal ($AA^* \neq A^*A$). Non-normality can lead to
  transient growth in the QR residual and further stall the convergence of
  sub-diagonal elements toward the Real Schur Form.

[^1]: Stewart, G. W., and J.-G. Sun. 1990. Matrix Perturbation Theory. Boston: Academic Press. §3.1.
