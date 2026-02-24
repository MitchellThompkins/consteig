# Verification

The library is verified through two primary methods:
1. **Eigen Library Comparison**: Unit tests link against the
   [Eigen](https://eigen.tuxfamily.org/) library to compare compile-time results
   against a high-performance reference implementation.
2. **Octave Test Generation**: An Octave script (`octave/generate_test_cases.m`)
   is provided to generate fresh matrix test data and expected results, which
   are automatically verified using `static_assert` at compile time.

## Accuracy [^3]

An eigenvalue solver is limited not only by the numerical algorithm it employs
but also by the conditioning of the eigenvalue problem and the finite precision
of the machine representation. For defective matrices, eigenvalues are
intrinsically ill-conditioned. That is, small perturbations to the matrix can
produce much larger perturbations in the eigenvalues.

For a defective eigenvalue associated with a single $N\times N$ Jordan block,
classical perturbation theory shows that eigenvalue perturbations scale as

```math
\delta \lambda| \sim \|E\|^{1/N}
```

Modern dense eigenvalue algorithms (e.g., QR) are backward stable, meaning
the perturbation they introduce is on the order of machine precision:

```math
\|E\|\lesssim \epsilon_{\text{mach}}\|A\|
```

which says

> the numerical error in the matrix is no larger than machine epsilon times the
size of the matrix.

If the matrix is scaled so that its norm is about 1:

```math
\|A\| \approx 1
```

then the backward error simplifies to:

```math
\|E\| \approx \epsilon_{\text{mach}}
```

So the solver is effectively perturbing the matrix at the level of machine
precision.

For IEEE-754 double precision $\epsilon_{\text{mach}}=2^{-53}$ and a single
$8\times8$ Jordan block this gives:

```math
(2^{-53})^{1\over8} \approx 0.010
```

Accordingly, for defective matrices of this type, eigenvalues cannot in general
be expected to be accurate beyond the percent level in double precision. The
observed accuracy of ~0.03 for consteig on such matrices is therefore
consistent with the theoretical limit.

## Compile-Time Verification Limits
Iterative algorithms like the QR iteration used here are computationally
expensive for a compiler's `constexpr` evaluator. To reliably verify 100+ test
cases without crashing the compiler or hitting operation limits.

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

The library performs rigorous eigenvalue verification using both
trace/determinant checks and direct comparison against reference values
(generated via Octave/LAPACK).

### Defective Matrices
For **defective matrices** (those with non-trivial Jordan blocks), the
eigenvalue problem is inherently ill-conditioned. A perturbation of size
$\epsilon$ in the matrix entries can result in a perturbation of size
$\epsilon^{1/k}$ in the eigenvalues, where $k$ is the size of the Jordan block.

For an $8 \times 8$ defective matrix (Jordan block of size 8) in double
precision ($\epsilon \approx 10^{-16}$):

```math
\text{Error} \approx (10^{-16})^{1/8} = 10^{-2} = 0.01
```

Consequently, tests for **defective**, **nearly defective**, and **large Jordan
block** matrices use a relaxed tolerance (`0.05`) to account for this
theoretical limit. This does not indicate a bug in the algorithm, but rather the
fundamental limit of computing eigenvalues for such matrices using standard
double-precision arithmetic.

### Standard Matrices
For normal, symmetric, and well-conditioned non-symmetric matrices, the library
maintains high-precision, typically matching reference values within `1e-9` or
better.

**Running Robustness Tests**:
```bash
make container.make.build
```

**CI/CD Integration**:
*   **Granular Binaries**: Non-symmetric test cases are split into individual
    `.cpp` files. This ensures each `static_assert` gets a fresh "budget" of
    compiler operations and limits the memory overhead to a single matrix solve
    at a time.
*   **Compiler Flags**: Unit test targets automatically raise limits like
    `-fconstexpr-ops-limit` locally to accommodate the depth of these
    calculations. For other targets, these limits are not modified by default to
    avoid unexpected side effects on user compiler configurations. Users can
    explicitly enable these raised limits globally by setting the
    `CONSTEIG_RAISE_COMPILER_LIMITS` CMake option to `ON`.
*   **Spectral Limits & Matrix Size Constraints**: While the algorithm is
    algebraically sound for higher orders, random matrices of orders higher than
    8x8 (and larger) frequently encounter particular arrangement, spacing, or
    clustering of eigenvalues in the matrix configurations. They thusly fail to
    converge within even an expanded `constexpr` operation budget (1BM+ steps).
    These limits are applied automatically to library tests, but users may need
    to explicitly enable them for their own targets using
    `CONSTEIG_RAISE_COMPILER_LIMITS=ON`.
*   From a numerical analysis perspective, the following factors have a
    significant impact:
    *   **Spectral Separation and Convergence Rate**: The convergence rate of
        the QR algorithm is intrinsically governed by the ratios of adjacent
        eigenvalues $|\lambda_{i+1}/\lambda_i|$ in the ordered spectrum. As the
        matrix dimension $n$ increases, the probability of encountering
        "clustered" eigenvalues (where $\lambda_i \approx \lambda_{i+1}$) rises
        sharply. In a random real 10x10 matrix, eigenvalues often distribute
        near the unit circle with several close pairs or nearly equal
        magnitudes.
    *   **The Unit Convergence Factor**: When eigenvalues are poorly separated,
        the convergence factor $\rho = |\lambda_{i+1}/\lambda_i|$ approaches
        unity. This results in slow linear convergence, requiring an explosive
        number of iterations to satisfy the deflation criterion
```math
|h_{k+1,k}| \le \epsilon (|h_{kk}| + |h_{k+1,k+1}|)
```
    *   **Non-Normal Structure**: Randomly generated matrices are typically
        highly non-normal ($AA^* \neq A^*A$). Non-normality can lead to
        transient growth in the QR residual and further stall the convergence of
        sub-diagonal elements toward the Real Schur Form.
    *   **Constexpr Step Budget**: While 500M operations is significantly higher
        than the compiler default (33,554,432 in GCC), it remains a finite
        constraint. A stalled QR iteration on a 10x10 non-normal matrix can
        easily exhaust this budget, causing a compilation failure.

[^3]: Stewart, G. W., and J.-G. Sun. 1990. Matrix Perturbation Theory. Boston: Academic Press. §3.1.
