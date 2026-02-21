#ifndef GENERATED_TEST_HELPERS_HPP
#define GENERATED_TEST_HELPERS_HPP

#include <gtest/gtest.h>

#include "../consteig.hpp"
#include "generated_cases.hpp"
#include "test_tools.hpp"

using namespace consteig;

// Define helper macros for consistency
#define GENERATE_CHECK(category, type, suffix, mat_var, eig_var, tol)                          \
    template <Size INDEX>                                                                      \
    constexpr bool check_single_##category##_##type##_##suffix() {                             \
        auto eigs = eigvals(mat_var[INDEX]);                                                   \
        if (!checkEigenValues(mat_var[INDEX], eigs, static_cast<double>(tol))) return false;   \
        if (!compareEigenValues(eigs, eig_var[INDEX], static_cast<double>(tol))) return false; \
        return true;                                                                           \
    }

// Random cases
#define RANDOM_FAST_TOL 1e-9
#define RANDOM_SLOW_TOL 1e-7
GENERATE_CHECK(random, sym, fast, mat_random_sym_fast, eigs_random_sym_fast, RANDOM_FAST_TOL)
GENERATE_CHECK(random, nonsym, fast, mat_random_nonsym_fast, eigs_random_nonsym_fast, RANDOM_FAST_TOL)
GENERATE_CHECK(random, sym, slow, mat_random_sym_slow, eigs_random_sym_slow, RANDOM_SLOW_TOL)
GENERATE_CHECK(random, nonsym, slow, mat_random_nonsym_slow, eigs_random_nonsym_slow, RANDOM_SLOW_TOL)

// Robustness cases
#define PATHOLOGICAL_FAST_TOL 0.05
#define LOOSE_FAST_TOL 1e-9
#define STRICT_FAST_TOL 1e-9

#define PATHOLOGICAL_SLOW_TOL 0.03
#define LOOSE_SLOW_TOL 1e-7
#define STRICT_SLOW_TOL 1e-8

#define GENERATE_ROBUST(category, fast_tol, slow_tol)                                                                       \
    GENERATE_CHECK(category, nonsym, fast, mat_##category##_nonsym_fast, eigs_##category##_nonsym_fast, fast_tol) \
    GENERATE_CHECK(category, nonsym, slow, mat_##category##_nonsym_slow, eigs_##category##_nonsym_slow, slow_tol)

GENERATE_ROBUST(defective, PATHOLOGICAL_FAST_TOL, PATHOLOGICAL_SLOW_TOL)
GENERATE_ROBUST(nearly_defective, PATHOLOGICAL_FAST_TOL, PATHOLOGICAL_SLOW_TOL)
GENERATE_ROBUST(large_jordan, PATHOLOGICAL_FAST_TOL, PATHOLOGICAL_SLOW_TOL)

GENERATE_ROBUST(non_normal, LOOSE_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(clustered, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(repeated, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(companion, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(graded, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(toeplitz, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(nearly_reducible, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(random_non_normal, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(hamiltonian, STRICT_FAST_TOL, STRICT_SLOW_TOL)
GENERATE_ROBUST(sparse_interior, STRICT_FAST_TOL, STRICT_SLOW_TOL)

#endif
