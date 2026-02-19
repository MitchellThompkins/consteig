#ifndef GENERATED_TEST_HELPERS_HPP
#define GENERATED_TEST_HELPERS_HPP

#include <gtest/gtest.h>

#include "../consteig.hpp"
#include "generated_cases.hpp"
#include "test_tools.hpp"

using namespace consteig;

template <typename T, Size S>
constexpr Complex<T> sum_eigs(const Matrix<Complex<T>, S, 1>& vec) {
    Complex<T> s{};
    for (Size i = 0; i < S; ++i) s = s + vec(i, 0);
    return s;
}

template <typename T, Size S>
constexpr bool verify_eigenvalues(const Matrix<Complex<T>, S, 1>& computed, const Matrix<Complex<T>, S, 1>& expected,
                                  T tol = static_cast<T>(CONSTEIG_TEST_TOLERANCE)) {
    bool matched[S] = {};
    for (Size i = 0; i < S; ++i) matched[i] = false;

    for (Size i = 0; i < S; ++i) {
        bool found = false;
        for (Size j = 0; j < S; ++j) {
            if (!matched[j]) {
                // For complex matching, use a simple Manhattan distance or similar
                T diff_real = computed(i, 0).real - expected(j, 0).real;
                T diff_imag = computed(i, 0).imag - expected(j, 0).imag;
                if (consteig::abs(diff_real) < tol && consteig::abs(diff_imag) < tol) {
                    matched[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

// Define helper macros for consistency
#define GENERATE_CHECK(category, type, suffix, mat_var, eig_var, tol)                          \
    template <Size INDEX>                                                                      \
    constexpr bool check_single_##category##_##type##_##suffix() {                             \
        auto eigs = eigvals(mat_var[INDEX]);                                                   \
        double tr = trace(mat_var[INDEX]);                                                     \
        auto s = sum_eigs(eigs);                                                               \
        if (!(consteig::abs(s.real - tr) < static_cast<double>(tol))) return false;            \
        if (!verify_eigenvalues(eigs, eig_var[INDEX], static_cast<double>(tol))) return false; \
        return true;                                                                           \
    }

// Random cases
GENERATE_CHECK(random, sym, fast, mat_random_sym_fast, eigs_random_sym_fast, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, nonsym, fast, mat_random_nonsym_fast, eigs_random_nonsym_fast, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, sym, slow, mat_random_sym_slow, eigs_random_sym_slow, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, nonsym, slow, mat_random_nonsym_slow, eigs_random_nonsym_slow, CONSTEIG_TEST_TOLERANCE)

// Robustness cases (looser tolerance for hard matrices)
#define ROBUST_TOL 1e-4

#define GENERATE_ROBUST(category)                                                                                   \
    GENERATE_CHECK(category, nonsym, fast, mat_##category##_nonsym_fast, eigs_##category##_nonsym_fast, ROBUST_TOL) \
    GENERATE_CHECK(category, nonsym, slow, mat_##category##_nonsym_slow, eigs_##category##_nonsym_slow, ROBUST_TOL)

GENERATE_ROBUST(defective)
GENERATE_ROBUST(nearly_defective)
GENERATE_ROBUST(non_normal)
GENERATE_ROBUST(clustered)
GENERATE_ROBUST(repeated)
GENERATE_ROBUST(companion)
GENERATE_ROBUST(graded)
GENERATE_ROBUST(large_jordan)
GENERATE_ROBUST(toeplitz)
GENERATE_ROBUST(nearly_reducible)
GENERATE_ROBUST(random_non_normal)
GENERATE_ROBUST(hamiltonian)
GENERATE_ROBUST(sparse_interior)

#endif
