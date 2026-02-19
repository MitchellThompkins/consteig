# Project Todo List

## Math Library (`math/functions/`)
- [ ] **Trigonometry**: Implement `sin`, `cos`, `tan`, `atan`, `atan2`.
- [ ] **Constants**: Define `PI` and machine `epsilon` (critical for convergence checks).
- [ ] **Rounding**: Implement `floor`, `ceil`, `round`.
- [ ] **Fixes**:
    - [ ] `sqrt.hpp`: Return NaN for negative inputs.
    - [ ] `pow.hpp`: Improve error handling and support unsigned types.

## Matrix Library (`matrix/`)
- [ ] **Decompositions**:
    - [x] **QR**:
        - [ ] Support non-square matrices (remove `R==C` assertion).
        - [x] Handle singularity cases gracefully (when `r(i,i) == 0`).
    - [x] **Hessenberg**: Verify/fix base case for small (2x2) matrices (currently returns zero matrix for P instead of Identity).
    - [ ] `LU` Decomposition.
    - [ ] `Cholesky` Decomposition.
    - [ ] `SVD` (Singular Value Decomposition).
- [ ] **Operations**:
    - [ ] **Inverse**: Implement matrix inversion.
    - [ ] **Rank**: Implement matrix rank.
    - [x] **Det**: Optimize determinant calculation (currently recursive $O(n!)$); use LU or QR.
    - [x] **Norm**: Add other norms (Infinity, 1-norm) besides Euclidean.
- [ ] **Fixes**:
    - [ ] `operations.hpp`: Fix scalar multiplication ambiguity (allow scalar on both sides cleanly).
    - [ ] `matrix.hpp`: Implement robust floating-point equality checks.

## Eigenvalue Solver (`eigen/`)
- [x] **Non-Symmetric Matrices (Stability Fix Required)**: 
    - The `eig_double_shifted_qr` implementation is improved using balancing and robust Wilkinson shifts. 
    - **Status**: Stable for small (up to 5x5) non-symmetric matrices in constexpr context.
- [ ] **Eigenvector Computation**: Implement a function to compute eigenvectors. Currently, `eig()` returns the Schur form, but extracting eigenvectors requires a back-substitution algorithm not yet present in the library.
- [x] **Convergence**: Improve convergence criteria using machine epsilon instead of hardcoded `1e-10`.
- [ ] **Testing**:
    - [ ] Add tests for `checkEigenValues`.
    - [x] Re-enable `generated_tests.nonsymmetric_constexpr`.

## General / Infrastructure
- [ ] **Build System**: Review `CMakeLists.txt` TODOs.
- [ ] **Documentation**: Update `readme.md` with current capabilities and usage examples.
- [ ] **CI/CD**: Ensure all tests run in the CI pipeline.

## Human Checks
- [ ] Validate functionality of complex numbers.
- [x] Add large matricies.
- [x] Address all `-wall` issues.
- [ ] Check sum_eigs, is it doing the right thing? Is that the det()?
- [ ] Are generated test cases actually generating the eigen values too? I think
  no.
- [ ] What happens if the matrix has complex values? I.e. for a polynomial if
  the roots aren't real, and QR iteration doens't converge to a diagonal, i
  think i'll get an irreducible 2x2 block whose eigen values are the complex
  conjugate pair. Do I handle that?
- [ ] Add more test cases for regular math as welll
- [ ] Explore if `-fconstexpr-ops-limit` is okay for test only or if all builds
  need it
- [ ] Compare CONSTEIG_MAX_ITER and fconstepxr-ops-limit and understand
  difference
- [ ] Check that symmetrical matrices are symmetrical
- [ ] Choose worst-cases that have eigenvals clustered togethered.
- [ ] Audit the readme to sound more like me
- [x] Add DSP example for filter coeffs
- [x] Stop building debug version
- [ ] Add clang-format or something to this repo
- [ ] Comment decompositions and eigen functions to understand what each step is
  doing. Maybe add referencs to text books.
- [ ] Explain usage of gram schmidt vs. francis in readme.
- [ ] Check that all tests actually perform a static assert to enforce compile
  time behaviors
- [ ] Check that fconstepxr-ops-limit is 1B for slow tests and 500M (or whatever
  is appropriate) for fast
  tests
- [ ] Check that sqrt(-1) returns complex number like it should (I think?)
- [ ] Write script to profile compile times as a function of matrix size
- [ ] Check that runtime tests are compared against eigen calculations for
  correctness
