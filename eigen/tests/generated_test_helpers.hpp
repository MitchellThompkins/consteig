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
GENERATE_CHECK(random, sym, fast, mat_random_sym_fast, eigs_random_sym_fast, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, nonsym, fast, mat_random_nonsym_fast, eigs_random_nonsym_fast, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, sym, slow, mat_random_sym_slow, eigs_random_sym_slow, CONSTEIG_TEST_TOLERANCE)
GENERATE_CHECK(random, nonsym, slow, mat_random_nonsym_slow, eigs_random_nonsym_slow, CONSTEIG_TEST_TOLERANCE)

// Robustness cases
#define LOOSE_TOL 0.05
#define STRICT_TOL 1e-3

#define GENERATE_ROBUST(category, tol)                                                                       \
    GENERATE_CHECK(category, nonsym, fast, mat_##category##_nonsym_fast, eigs_##category##_nonsym_fast, tol) \
    GENERATE_CHECK(category, nonsym, slow, mat_##category##_nonsym_slow, eigs_##category##_nonsym_slow, tol)

GENERATE_ROBUST(defective, LOOSE_TOL)
GENERATE_ROBUST(nearly_defective, LOOSE_TOL)
GENERATE_ROBUST(large_jordan, LOOSE_TOL)

GENERATE_ROBUST(non_normal, STRICT_TOL)
GENERATE_ROBUST(clustered, STRICT_TOL)
GENERATE_ROBUST(repeated, STRICT_TOL)
GENERATE_ROBUST(companion, STRICT_TOL)
GENERATE_ROBUST(graded, STRICT_TOL)
GENERATE_ROBUST(toeplitz, STRICT_TOL)
GENERATE_ROBUST(nearly_reducible, STRICT_TOL)
GENERATE_ROBUST(random_non_normal, STRICT_TOL)
GENERATE_ROBUST(hamiltonian, STRICT_TOL)
GENERATE_ROBUST(sparse_interior, STRICT_TOL)

#endif
